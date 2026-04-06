/**
 * @file hw_config_03.h
 * @brief Mapeo físico para el Contador Industrial Externo.
 */

#ifndef HW_CONFIG_03_H
#define HW_CONFIG_03_H

#include "gpio.h"

/* --- Mapeo del Display de 7 Segmentos (Puerto 6) --- */
// Nota: Todos en SCU Puerto 6, Función FUNC0 para GPIO
static const gpio_config_t seg_a  = {6, 1,  SCU_MODE_FUNC0, 3, 0};  // P6_1  -> GPIO3[0]
static const gpio_config_t seg_b  = {6, 5,  SCU_MODE_FUNC0, 3, 4};  // P6_5  -> GPIO3[4]
static const gpio_config_t seg_c  = {6, 8,  SCU_MODE_FUNC4, 5, 16}; // P6_8  -> GPIO5[16]
static const gpio_config_t seg_d  = {6, 10, SCU_MODE_FUNC0, 3, 6};  // P6_10 -> GPIO3[6]
static const gpio_config_t seg_e  = {6, 4,  SCU_MODE_FUNC0, 3, 3};  // P6_4  -> GPIO3[3]
static const gpio_config_t seg_f  = {6, 7,  SCU_MODE_FUNC4, 5, 15}; // P6_7  -> GPIO5[15]
static const gpio_config_t seg_g  = {6, 9,  SCU_MODE_FUNC0, 3, 5};  // P6_9  -> GPIO3[5]
static const gpio_config_t seg_dp = {6, 11, SCU_MODE_FUNC0, 3, 7};  // P6_11 -> GPIO3[7]

/* --- Mapeo de Pulsadores Externos (Lógica Negativa) --- */
static const gpio_config_t btn_start = {1, 5, SCU_MODE_FUNC0, 1, 8}; // P1_5 -> GPIO1[8]
static const gpio_config_t btn_reset = {4, 2, SCU_MODE_FUNC0, 2, 2}; // P4_2 -> GPIO2[2]

/* Arreglo para inicialización masiva en el driver del display */
static const gpio_config_t display_pins[] = {
    seg_a, seg_b, seg_c, seg_d, seg_e, seg_f, seg_g, seg_dp
};

#endif