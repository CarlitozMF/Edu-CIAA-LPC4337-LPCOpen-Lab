/**
 * @file led.h
 * @brief API de abstracción para el manejo de LEDs.
 */

#ifndef LED_H
#define LED_H

#include "gpio.h"

/**
 * @brief Inicializa un array de LEDs como salidas.
 * @param table Puntero al array de configuraciones GPIO.
 * @param count Cantidad de LEDs en el array.
 */
void LED_Init_Array(const gpio_config_t *table, uint8_t count);

/**
 * @brief Cambia el estado de un LED específico por su índice.
 * @param table Puntero al array de configuraciones.
 * @param index Índice del LED en el array.
 * @param state Estado (GPIO_HIGH / GPIO_LOW).
 */
void LED_Set(const gpio_config_t *table, uint8_t index, gpio_state_t state);

/**
 * @brief Alterna el estado de un LED específico.
 * @param table Puntero al array de configuraciones.
 * @param index Índice del LED en el array.
 */
void LED_Toggle(const gpio_config_t *table, uint8_t index);

#endif