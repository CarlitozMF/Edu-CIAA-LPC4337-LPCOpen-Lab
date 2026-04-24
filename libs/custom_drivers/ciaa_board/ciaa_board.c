/**
 * @file ciaa_board.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Implementación del Board Support Package (BSP) para la EDU-CIAA LPC4337.
 * * Este módulo actúa como una capa de enlace entre el hardware físico de la placa
 * y el driver HAL de GPIO. Centraliza el mapeo de registros SCU y GPIO 
 * correspondientes a los periféricos integrados.
 */

#include "ciaa_board.h"

/* --- Tablas de Mapeo de Hardware (Privadas) --- */

/**
 * @brief Mapeo de LEDs integrados.
 * @note El LED RGB requiere FUNC4 para operar como GPIO, mientras que los 
 * LEDs monocolor operan bajo FUNC0.
 */
static const gpio_config_t boardLeds[] = {
    [CIAA_LED_R] = {2, 0,  SCU_MODE_FUNC4, 5, 0},  /**< P2_0,  FUNC4, GPIO 5[0] */
    [CIAA_LED_G] = {2, 1,  SCU_MODE_FUNC4, 5, 1},  /**< P2_1,  FUNC4, GPIO 5[1] */
    [CIAA_LED_B] = {2, 2,  SCU_MODE_FUNC4, 5, 2},  /**< P2_2,  FUNC4, GPIO 5[2] */
    [CIAA_LED_1] = {2, 10, SCU_MODE_FUNC0, 0, 14}, /**< P2_10, FUNC0, GPIO 0[14] */
    [CIAA_LED_2] = {2, 11, SCU_MODE_FUNC0, 1, 11}, /**< P2_11, FUNC0, GPIO 1[11] */
    [CIAA_LED_3] = {2, 12, SCU_MODE_FUNC0, 1, 12}  /**< P2_12, FUNC0, GPIO 1[12] */
};

/**
 * @brief Mapeo de Pulsadores (TECs).
 * @note Todos mapeados bajo FUNC0. El hardware cuenta con pull-ups externos,
 * pero se habilita el interno por redundancia y robustez.
 */
static const gpio_config_t boardTecs[] = {
    [CIAA_TEC_1] = {1, 0, SCU_MODE_FUNC0, 0, 4}, /**< P1_0, FUNC0, GPIO 0[4] */
    [CIAA_TEC_2] = {1, 1, SCU_MODE_FUNC0, 0, 8}, /**< P1_1, FUNC0, GPIO 0[8] */
    [CIAA_TEC_3] = {1, 2, SCU_MODE_FUNC0, 0, 9}, /**< P1_2, FUNC0, GPIO 0[9] */
    [CIAA_TEC_4] = {1, 6, SCU_MODE_FUNC0, 1, 9}  /**< P1_6, FUNC0, GPIO 1[9] */
};

/* --- Funciones Públicas --- */

/**
 * @brief Inicializa el hardware integrado de la placa.
 * Itera sobre las tablas de mapeo aplicando la configuración inicial.
 */
void CIAA_Board_Init(void) {
    /* Configuración de LEDs: Salidas con estado inicial LOW (Seguridad) */
    for (int i = 0; i < CIAA_LEDS_MAX; i++) {
        GPIO_Init(&boardLeds[i], GPIO_OUTPUT);
    }

    /* Configuración de TECs: Entradas con Pull-Up y Buffer habilitado */
    for (int i = 0; i < CIAA_TECS_MAX; i++) {
        GPIO_Init(&boardTecs[i], GPIO_INPUT_PULLUP);
    }
}

/**
 * @brief Escribe un estado en un LED por índice.
 * Incluye validación de límites para evitar accesos fuera de memoria.
 */
void CIAA_LED_Set(ciaa_led_t led, gpio_state_t state) {
    if (led < CIAA_LEDS_MAX) {
        GPIO_Write(&boardLeds[led], state);
    }
}

/**
 * @brief Invierte el estado de un LED por índice.
 */
void CIAA_LED_Toggle(ciaa_led_t led) {
    if (led < CIAA_LEDS_MAX) {
        GPIO_Toggle(&boardLeds[led]);
    }
}

/**
 * @brief Lee un pulsador por índice.
 * @return Estado lógico (GPIO_LOW si está presionado).
 */
gpio_state_t CIAA_TEC_Get(ciaa_tec_t tec) {
    if (tec < CIAA_TECS_MAX) {
        return GPIO_Read(&boardTecs[tec]);
    }
    return GPIO_HIGH; /* Estado 'no presionado' seguro ante error de índice */
}