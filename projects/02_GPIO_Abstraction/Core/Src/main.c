/**
 * @file main.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Aplicación principal: Orquestación de I/O mediante drivers abstractos.
 */


#include "main_project_02.h"

/**
 * @brief Función principal del sistema.
 */
int main(void) {
    /* 1. Sincronización de hardware inicial */
    SystemCoreClockUpdate();

    /* 2. Inicialización de Capa 2 (Drivers) 
       Pasamos el array definido en hardware.h y su tamaño */
    LED_Init_Array(ledsExternos, CANT_LEDS);
    LED_Init_Array(ledsRGB, CANT_LEDSRGB);
    RGBOFF();

    /* 4. Bucle infinito (Lógica de negocio) */
    while (1) {

    // Encendido secuencial
    for (uint8_t i = 0; i < CANT_LEDS; i++) {
        LED_Set(ledsExternos, i, GPIO_HIGH);
        delay(SystemCoreClock / 40);
    }
    // Apagado secuencial inverso
    for (int8_t i = (CANT_LEDS - 1); i >= 0; i--) {
        LED_Set(ledsExternos, i, GPIO_LOW);
        delay(SystemCoreClock / 40);
    }
    }

    return 0;
}

/* --- Implementación de Funciones Locales --- */
void RGBOFF(){
    for (uint8_t i = 0; i < CANT_LEDSRGB; i++)
    {
        LED_Set(ledsRGB,i,GPIO_LOW);
    }
    
}

static void delay(uint32_t count) {
    for (volatile uint32_t i = 0; i < count; i++);
}