/**
 * @file main.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Aplicación principal: Orquestación de I/O mediante SysTick y Drivers.
 */

#include "hardware.h"
#include "sys_core.h"
#include "led.h"

int main(void) {
  
/* 1. INICIALIZACIÓN DEL SISTEMA*/
    Sys_Init();

/* --- Variables de estado (Capa de Aplicación) --- */
//Variables para el toggle por boton
static bool btn_prev_state = true;    // Pull-up: Reposo es HIGH
uint32_t btn_press_start_tick = 0;
const uint32_t BUTTON_DEBOUNCE_MS = 50;

// Variables para LED1 (Rojo)
uint32_t lastToggleTime1 = 0;
//GPIO_PinState ledState1 = GPIO_PIN_RESET;
const uint32_t togglePeriod1 = 1000; // 1000 ms de alternancia

// Variables para LED2 (Amarillo)
uint32_t lastToggleTime2 = 0;
//GPIO_PinState ledState2 = GPIO_PIN_RESET;
const uint32_t togglePeriod2 = 500; // 300 ms de alternancia

// Variables para LED3 (Verde)
uint32_t lastToggleTime3 = 0;
//GPIO_PinState ledState3 = GPIO_PIN_RESET;
const uint32_t togglePeriod3 = 250; // 600 ms de alternancia

    while (1) {

        // TAREA 1: Entrada/Salida inmediata (Polling de botón)
        bool btn_now = (CIAA_TEC_Get(CIAA_TEC_1) == GPIO_LOW);
        // 2. DETECCIÓN DE FLANCO DE BAJADA (Cuando el dedo toca el botón)
        if (btn_now && !btn_prev_state) {
            btn_press_start_tick = SysTick_GetTicks();
        }

    // 3. DETECCIÓN DE FLANCO DE SUBIDA (Cuando el dedo SUELTA el botón)
        if (!btn_now && btn_prev_state) {
        // Validación por tiempo: ¿Estuvo presionado lo suficiente?
            if ((SysTick_GetTicks() - btn_press_start_tick) >= BUTTON_DEBOUNCE_MS) {
            
                CIAA_LED_Toggle(CIAA_LED_G);
            }
        }

    // 4. Actualizar estado anterior
        btn_prev_state = btn_now;

        // TAREA 2: Tarea temporizada (Heartbeat de 250ms)
        // Verificamos el tiempo transcurrido sin detener el CPU
        if ((SysTick_GetTicks() - lastToggleTime1) >= togglePeriod1) {
            lastToggleTime1 = SysTick_GetTicks();
            CIAA_LED_Toggle(CIAA_LED_1);
        }


        // TAREA 3: Tarea temporizada (Heartbeat de 500ms)
        // Verificamos el tiempo transcurrido sin detener el CPU
        if ((SysTick_GetTicks() - lastToggleTime2) >= togglePeriod2) {
            lastToggleTime2 = SysTick_GetTicks();
            CIAA_LED_Toggle(CIAA_LED_2);
        }

        // TAREA 4: Tarea temporizada (Heartbeat de 1000ms)
        // Verificamos el tiempo transcurrido sin detener el CPU
        if ((SysTick_GetTicks() - lastToggleTime3) >= togglePeriod3) {
            lastToggleTime3 = SysTick_GetTicks();
            CIAA_LED_Toggle(CIAA_LED_3);
        }

    }

    return 0;
}

