/**
 * @file led.h
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief API de Abstracción de Alto Nivel para la gestión de arreglos de LEDs.
 * Proporciona una interfaz semántica para controlar múltiples LEDs mediante índices,
 * abstrayendo la configuración física de los pines GPIO.
 * @version 1.0
 * @date 2026-04-04
 */

#ifndef LED_H
#define LED_H

#include "gpio.h"

/* --- API de Gestión de LEDs --- */

/**
 * @brief Inicializa un conjunto de LEDs configurándolos como salidas digitales.
 * @note Establece un estado inicial seguro (GPIO_LOW) para todos los elementos.
 * @param table Puntero al array de estructuras gpio_config_t que definen los pines.
 * @param count Cantidad total de LEDs presentes en el arreglo.
 */
void LED_Init_Array(const gpio_config_t *table, uint8_t count);

/**
 * @brief Establece el estado lógico de un LED específico dentro del arreglo.
 * @param table Puntero al array de configuraciones GPIO.
 * @param index Índice del LED deseado (empezando en 0).
 * @param state Estado a escribir (GPIO_HIGH para encender, GPIO_LOW para apagar).
 */
void LED_Set(const gpio_config_t *table, uint8_t index, gpio_state_t state);

/**
 * @brief Invierte el estado actual (Toggle) de un LED específico por su índice.
 * @param table Puntero al array de configuraciones GPIO.
 * @param index Índice del LED en el arreglo.
 */
void LED_Toggle(const gpio_config_t *table, uint8_t index);

#endif /* LED_H */