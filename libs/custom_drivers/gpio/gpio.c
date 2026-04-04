/**
 * @file gpio.c
 * @brief Implementación del driver HAL para el periférico GPIO.
 */

#include "gpio.h"

void GPIO_Init(const gpio_config_t *config, gpio_mode_t mode) {
    uint16_t scuMode = SCU_MODE_INACT;

    // Configuración de la SCU basándose en el modo solicitado
    switch (mode) {
        case GPIO_INPUT_PULLUP:   scuMode = SCU_MODE_PULLUP;   break;
        case GPIO_INPUT_PULLDOWN: scuMode = SCU_MODE_PULLDOWN; break;
        case GPIO_INPUT_REPEATER: scuMode = SCU_MODE_REPEATER; break;
        default:                  scuMode = SCU_MODE_INACT;    break;
    }

    // 1. Configurar Matriz de Conmutación (Muxing)
    Chip_SCU_PinMuxSet(config->scuPort, config->scuPin, (scuMode | config->scuFunc));

    // 2. Configurar Dirección y estado inicial
    if (mode == GPIO_OUTPUT) {
        Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
        GPIO_Write(config, GPIO_LOW); // Estado seguro: siempre inicia en LOW
    } else {
        Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
    }
}

void GPIO_Write(const gpio_config_t *config, gpio_state_t state) {
    // Usamos el casting para que el driver de LPCOpen reciba el valor correcto
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, config->gpioPort, config->gpioPin, (bool)state);
}

gpio_state_t GPIO_Read(const gpio_config_t *config) {
    return (gpio_state_t)Chip_GPIO_GetPinState(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
}

void GPIO_Toggle(const gpio_config_t *config) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, config->gpioPort, config->gpioPin);
}