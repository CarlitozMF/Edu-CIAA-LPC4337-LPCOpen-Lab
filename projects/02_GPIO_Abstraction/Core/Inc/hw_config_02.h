#ifndef HW_CONFIG_02_H
#define HW_CONFIG_02_H

#include "gpio.h"

/**
 * @brief Mapeo de hardware para el Laboratorio 02.
 * El uso de 'static const' asegura que el array viva en FLASH y sea local al modulo.
 */
static const gpio_config_t ledsExternos[] = {
    {2, 10, SCU_MODE_FUNC0, 0, 14}, /**< LED_EXT_1: Pin P2_10 -> GPIO0[14] */
    {2, 11, SCU_MODE_FUNC0, 1, 11}, /**< LED_EXT_2: Pin P2_11 -> GPIO1[11] */
    {2, 12, SCU_MODE_FUNC0, 1, 12}, /**< LED_EXT_3: Pin P2_12 -> GPIO1[12] */
};

/** @brief Cálculo dinámico de la cantidad de LEDs */
#define CANT_LEDS (sizeof(ledsExternos) / sizeof(ledsExternos[0]))


static const gpio_config_t ledsRGB[] = {
    {2, 0, SCU_MODE_FUNC4, 5, 0}, /**< LED_RGBR: Pin P2_0 -> GPIO5[0] */
    {2, 1, SCU_MODE_FUNC4, 5, 1}, /**< LED_RGBG: Pin P2_1 -> GPIO5[1] */
    {2, 2, SCU_MODE_FUNC4, 5, 2}, /**< LED_RGBB: Pin P2_2 -> GPIO5[2] */
};

#define CANT_LEDSRGB (sizeof(ledsRGB) / sizeof(ledsRGB[0]))

#endif /* HW_CONFIG_02_H */