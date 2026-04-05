/**
 * @file gpio.h
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Driver de Abstracción de Bajo Nivel (HAL) para GPIO en LPC4337.
 * * Este driver gestiona la compleja relación entre la Matriz de Conmutación (SCU)
 * y el periférico GPIO del LPC4337, permitiendo una configuración atómica y 
 * semántica de los pines.
 * * @version 1.0
 * @date 2026-04-04
 */

#ifndef GPIO_H
#define GPIO_H

#include "chip.h"

/**
 * @brief Etiquetas semánticas para estados lógicos de los pines.
 * Se utilizan para evitar el uso de literales (0/1) en la capa de aplicación.
 */
typedef enum {
    GPIO_LOW = 0,  /**< Estado lógico bajo (0V - Tierra) */
    GPIO_HIGH = 1  /**< Estado lógico alto (3.3V - VCC) */
} gpio_state_t;

/**
 * @brief Modos de configuración de entrada/salida y resistencias internas.
 * Define el comportamiento eléctrico del pin en la SCU antes de asignar su dirección.
 */
typedef enum {
    GPIO_INPUT = 0,       /**< Entrada digital de alta impedancia (Hi-Z) */
    GPIO_OUTPUT,          /**< Salida digital (Push-Pull) */
    GPIO_INPUT_PULLUP,    /**< Entrada con resistencia de Pull-Up (~50kOhm) activa */
    GPIO_INPUT_PULLDOWN,  /**< Entrada con resistencia de Pull-Down (~50kOhm) activa */
    GPIO_INPUT_REPEATER   /**< Modo Bus-Keeper: mantiene el último estado lógico detectado */
} gpio_mode_t;

/**
 * @brief Estructura de mapeo de hardware para un pin físico.
 * Contiene la "doble identidad" del pin en el LPC4337: su ubicación en la SCU
 * y su nombre en el periférico GPIO.
 */
typedef struct {
    uint8_t  scuPort;   /**< Puerto de la Matriz de Conmutación (SCU) [0..15] */
    uint8_t  scuPin;    /**< Pin de la Matriz de Conmutación (SCU) [0..31] */
    uint16_t scuFunc;   /**< Función del pin (ej. SCU_MODE_FUNC0, SCU_MODE_FUNC4) */
    uint8_t  gpioPort;  /**< Puerto del periférico GPIO [0..7] */
    uint8_t  gpioPin;   /**< Pin del periférico GPIO [0..31] */
} gpio_config_t;

/* --- Prototipos de Funciones --- */

/**
 * @brief Inicializa un pin configurando SCU y Dirección GPIO.
 * * Esta función realiza tres tareas críticas:
 * 1. Muxeo del pin en la SCU con la función y modo indicados.
 * 2. Habilitación del Input Buffer (necesario para lecturas digitales).
 * 3. Configuración del registro de dirección (DIR) en el periférico GPIO.
 * * @param config Puntero a la estructura de configuración constante del pin.
 * @param mode Modo de operación deseado (Input/Output/Pulls).
 */
void GPIO_Init(const gpio_config_t *config, gpio_mode_t mode);

/**
 * @brief Escribe un estado lógico en un pin de salida.
 * * Utiliza el acceso atómico a los registros del GPIO para garantizar
 * que la operación no interfiera con otros pines del mismo puerto.
 * * @param config Puntero a la estructura de configuración del pin.
 * @param state Estado a escribir (GPIO_HIGH o GPIO_LOW).
 */
void GPIO_Write(const gpio_config_t *config, gpio_state_t state);

/**
 * @brief Lee el estado lógico actual de un pin.
 * * Requiere que el pin haya sido inicializado con el Input Buffer habilitado 
 * en la SCU (gestionado automáticamente por GPIO_Init).
 * * @param config Puntero a la estructura de configuración del pin.
 * @return gpio_state_t Estado leído (GPIO_HIGH o GPIO_LOW).
 */
gpio_state_t GPIO_Read(const gpio_config_t *config);

/**
 * @brief Alterna el estado lógico de un pin (Toggle).
 * * Realiza una operación de inversión de estado sin necesidad de lecturas
 * previas por software, optimizando el tiempo de CPU.
 * * @param config Puntero a la estructura de configuración del pin.
 */
void GPIO_Toggle(const gpio_config_t *config);

#endif /* GPIO_H */