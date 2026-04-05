/**
 * @file ciaa_board.h
 * @author Carlos Mamamni Flores (UTN-FRT)
 * @brief Board Support Package (BSP) para la EDU-CIAA LPC4337.
 * * Este módulo abstrae los periféricos integrados de la placa (LEDs y Pulsadores)
 * utilizando el driver de GPIO de Capa 2. Permite una gestión semántica del
 * hardware sin necesidad de conocer el mapeo de pines en cada llamado.
 * * @version 1.1
 * @date 2026-04-04
 */

#ifndef CIAA_BOARD_H
#define CIAA_BOARD_H

#include "gpio.h"

/**
 * @brief Enumeración de LEDs integrados en la EDU-CIAA.
 * * Incluye tanto el LED RGB (mapeado vía SCU FUNC4) como los 
 * LEDs de usuario monocolor (mapeados vía SCU FUNC0).
 */
typedef enum {
    CIAA_LED_R = 0, /**< LED RGB Rojo   (P2_0,  GPIO 5[0]) */
    CIAA_LED_G,     /**< LED RGB Verde  (P2_1,  GPIO 5[1]) */
    CIAA_LED_B,     /**< LED RGB Azul   (P2_2,  GPIO 5[2]) */
    CIAA_LED_1,     /**< LED 1 Rojo     (P2_10, GPIO 0[14]) */
    CIAA_LED_2,     /**< LED 2 Amarillo (P2_11, GPIO 1[11]) */
    CIAA_LED_3,     /**< LED 3 Verde    (P2_12, GPIO 1[12]) */
    CIAA_LEDS_MAX   /**< Límite para iteraciones */
} ciaa_led_t;

/**
 * @brief Enumeración de Pulsadores (TECs) integrados en la EDU-CIAA.
 * * Todos los pulsadores están configurados como Entradas con Pull-Up interno
 * y lógica negativa (presionado = LOW).
 */
typedef enum {
    CIAA_TEC_1 = 0, /**< Pulsador 1 (P1_0, GPIO 0[4]) */
    CIAA_TEC_2,     /**< Pulsador 2 (P1_1, GPIO 0[8]) */
    CIAA_TEC_3,     /**< Pulsador 3 (P1_2, GPIO 0[9]) */
    CIAA_TEC_4,     /**< Pulsador 4 (P1_6, GPIO 1[9]) */
    CIAA_TECS_MAX   /**< Límite para iteraciones */
} ciaa_tec_t;

/* --- Prototipos de Funciones de Placa --- */

/**
 * @brief Inicializa todos los periféricos integrados de la EDU-CIAA.
 * * Configura los LEDs como salidas (inicialmente LOW) y los pulsadores 
 * como entradas con Pull-Up habilitado y Buffer de entrada activo.
 */
void CIAA_Board_Init(void);

/**
 * @brief Controla el estado de un LED específico.
 * * @param led Identificador del LED (ciaa_led_t).
 * @param state Estado deseado (GPIO_HIGH para encender, GPIO_LOW para apagar).
 */
void CIAA_LED_Set(ciaa_led_t led, gpio_state_t state);

/**
 * @brief Alterna el estado actual de un LED (Toggle).
 * * @param led Identificador del LED (ciaa_led_t).
 */
void CIAA_LED_Toggle(ciaa_led_t led);

/**
 * @brief Lee el estado actual de un pulsador integrado.
 * * @note Debido a la configuración de hardware, devuelve GPIO_LOW 
 * cuando el botón está siendo presionado físicamente.
 * * @param tec Identificador del pulsador (ciaa_tec_t).
 * @return gpio_state_t Estado lógico actual (GPIO_LOW / GPIO_HIGH).
 */
gpio_state_t CIAA_TEC_Get(ciaa_tec_t tec);



#endif /* CIAA_BOARD_H */