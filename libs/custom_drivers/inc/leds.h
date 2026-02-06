#ifndef LEDS_H
#define LEDS_H

#include "stdint.h"
#include "stdbool.h"

// Enumeración para identificar los LEDs de la EDU-CIAA
typedef enum {
    LED_3,      // Amarillo
    LED_2,      // Rojo
    LED_1,      // Verde
    LED_RGB_R,
    LED_RGB_G,
    LED_RGB_B
} led_name_t;

// Prototipos de funciones
void led_init(void);
void led_write(led_name_t led, bool state);
void led_toggle(led_name_t led);

#endif