/**
 * @file gpio.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Implementación del driver HAL para el periférico GPIO en LPC4337.
 * * Este driver abstrae la complejidad de la Matriz de Conmutación (SCU) y 
 * proporciona funciones atómicas para el manejo de pines digitales.
 */

#include "gpio.h"

/**
 * @brief Inicializa un pin físico configurando SCU y dirección de datos.
 * * El proceso sigue la jerarquía del hardware:
 * 1. Selección de resistencias (Pulls) y modo eléctrico.
 * 2. Activación del receptor (Input Buffer).
 * 3. Mapeo de la función (Muxing).
 * 4. Configuración del sentido de datos (DIR) y estado inicial.
 */
void GPIO_Init(const gpio_config_t *config, gpio_mode_t mode) {
    uint16_t scuMode = SCU_MODE_INACT;

    /* 1. Determinación del modo eléctrico de la SCU */
    switch (mode) {
        case GPIO_INPUT_PULLUP:   scuMode = SCU_MODE_PULLUP;   break;
        case GPIO_INPUT_PULLDOWN: scuMode = SCU_MODE_PULLDOWN; break;
        case GPIO_INPUT_REPEATER: scuMode = SCU_MODE_REPEATER; break;
        case GPIO_INPUT:          scuMode = SCU_MODE_INACT;    break;
        case GPIO_OUTPUT:         scuMode = SCU_MODE_INACT;    break;
        default:                  scuMode = SCU_MODE_INACT;    break;
    }

    /* 2. Habilitación del Buffer de Entrada
     * Obligatorio en LPC43xx para que el registro de lectura (PIN) refleje 
     * el estado real del pin, incluso si está configurado como salida. */
    scuMode |= SCU_MODE_INBUFF_EN; 

    /* 3. Configuración de la Matriz de Conmutación (SCU)
     * Se combina el modo eléctrico con la función específica del pin. */
    Chip_SCU_PinMuxSet(config->scuPort, config->scuPin, (scuMode | config->scuFunc));

    /* 4. Configuración del periférico GPIO (Dirección) */
    if (mode == GPIO_OUTPUT) {
        /* Configura como salida y fuerza un estado lógico bajo por seguridad */
        Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
        GPIO_Write(config, GPIO_LOW); 
    } else {
        /* Configura como entrada de alta impedancia */
        Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
    }
}

/**
 * @brief Escribe un estado lógico (0 o 1) en un pin de salida.
 * Utiliza el registro de escritura de bits de LPCOpen para asegurar atomicidad.
 */
void GPIO_Write(const gpio_config_t *config, gpio_state_t state) {
    /* Casting a bool para compatibilidad con la firma de LPCOpen */
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, config->gpioPort, config->gpioPin, (bool)state);
}

/**
 * @brief Lee el estado lógico actual detectado en el pin.
 * @return gpio_state_t (GPIO_HIGH o GPIO_LOW).
 */
gpio_state_t GPIO_Read(const gpio_config_t *config) {
    /* Casting del retorno para mantener la integridad de tipos de nuestra HAL */
    return (gpio_state_t)Chip_GPIO_GetPinState(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
}

/**
 * @brief Alterna (Toggle) el estado actual del pin.
 * Útil para señalización, PWM por software o debugeo rápido.
 */
void GPIO_Toggle(const gpio_config_t *config) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
}