#include "led.h"

void LED_Init_Array(const gpio_config_t *table, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        GPIO_Init(&table[i], GPIO_OUTPUT);
    }
}

void LED_Set(const gpio_config_t *table, uint8_t index, gpio_state_t state) {
    // Aquí podrías agregar una validación de índice si fuera necesario
    GPIO_Write(&table[index], state);
}

void LED_Toggle(const gpio_config_t *table, uint8_t index) {
    GPIO_Toggle(&table[index]);
}