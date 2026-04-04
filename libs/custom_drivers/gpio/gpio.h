/**
 * @file gpio.h
 * @author Carlitoz MF & Gemini
 * @brief Driver de Abstracción de Bajo Nivel (HAL) para GPIO en LPC4337.
 * @version 1.0
 * @date 2026-02-12
 */

#ifndef GPIO_H
#define GPIO_H

#include "chip.h"


/**
 * @brief Etiquetas semánticas para estados lógicos de los pines.
 */
typedef enum {
    GPIO_LOW = 0,  /**< Estado lógico bajo (0V) */
    GPIO_HIGH = 1  /**< Estado lógico alto (3.3V) */
} gpio_state_t;

/**
 * @brief Modos de configuración de entrada/salida y resistencias internas.
 */
typedef enum {
    GPIO_INPUT = 0,      /**< Entrada sin resistencias */
    GPIO_OUTPUT,         /**< Salida Digital */
    GPIO_INPUT_PULLUP,   /**< Entrada con Pull-Up interno */
    GPIO_INPUT_PULLDOWN, /**< Entrada con Pull-Down interno */
    GPIO_INPUT_REPEATER  /**< Entrada en modo repetidor */
} gpio_mode_t;

/**
 * @brief Estructura de mapeo de hardware para un pin físico.
 */
typedef struct {
    uint8_t  scuPort;   /**< Puerto de la Matriz de Conmutación (SCU) */
    uint8_t  scuPin;    /**< Pin de la Matriz de Conmutación (SCU) */
    uint16_t scuFunc;   /**< Función del pin (ej. SCU_MODE_FUNC0) */
    uint8_t  gpioPort;  /**< Puerto del periférico GPIO */
    uint8_t  gpioPin;   /**< Pin del periférico GPIO */
} gpio_config_t;

/* --- Prototipos de Funciones --- */

/**
 * @brief Inicializa un pin configurando SCU y Dirección GPIO.
 * @param config Puntero a la estructura de configuración del pin.
 * @param mode Modo de operación (Input/Output/Pulls).
 */
void GPIO_Init(const gpio_config_t *config, gpio_mode_t mode);

/**
 * @brief Escribe un estado lógico en un pin de salida.
 * @param config Puntero a la estructura de configuración del pin.
 * @param state Estado a escribir (GPIO_HIGH o GPIO_LOW).
 */
void GPIO_Write(const gpio_config_t *config, gpio_state_t state);

/**
 * @brief Lee el estado lógico actual de un pin.
 * @param config Puntero a la estructura de configuración del pin.
 * @return gpio_state_t Estado leído (GPIO_HIGH o GPIO_LOW).
 */
gpio_state_t GPIO_Read(const gpio_config_t *config);

/**
 * @brief Alterna el estado lógico de un pin (Toggle).
 * @param config Puntero a la estructura de configuración del pin.
 */
void GPIO_Toggle(const gpio_config_t *config);

#endif