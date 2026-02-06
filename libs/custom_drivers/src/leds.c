#include "chip.h" // Cabecera principal de LPCOpen
#include "leds.h"

/* Si LPC_GPIO_PORT no está definido en tu versión, 
   usamos LPC_GPIO que es el estándar de NXP */
#ifndef LPC_GPIO_PORT
#define LPC_GPIO_PORT LPC_GPIO
#endif

// Estructura para el mapeo Capa 1 (Hardware)
typedef struct {
    uint8_t scu_p;   // Puerto SCU
    uint8_t scu_n;   // Pin SCU
    uint8_t gpio_p;  // Puerto GPIO
    uint8_t gpio_n;  // Pin GPIO
} led_config_t;

// Mapeo completo de la EDU-CIAA
static const led_config_t leds_map[] = {
    {2, 10, 5, 0}, // LED_1 (Verde)
    {2, 11, 5, 1}, // LED_2 (Rojo)
    {2, 12, 5, 2}, // LED_3 (Amarillo)
    {2, 0,  5, 0}, // LED_RGB_R (Puerto 5_0 es compartido o mapeado según HW)
    {2, 1,  5, 1}, 
    {2, 2,  5, 2}
};

void led_init(void) {
    SystemCoreClockUpdate(); // Inicializa relojes del sistema
    
    // Ejemplo: Configurar LED 3
    Chip_SCU_PinMuxSet(leds_map[LED_3].scu_p, leds_map[LED_3].scu_n, (SCU_MODE_INACT | SCU_MODE_FUNC0));
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, leds_map[LED_3].gpio_p, leds_map[LED_3].gpio_n);
}

void led_write(led_name_t led, bool state) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, leds_map[led].gpio_p, leds_map[led].gpio_n, state);
}