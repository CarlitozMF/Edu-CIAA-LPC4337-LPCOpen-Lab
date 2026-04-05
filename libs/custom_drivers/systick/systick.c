/**
 * @file systick.c
 * @author Carlos Mamani Flores (UTN-FRT) & Gemini
 * @brief Implementación robusta del SysTick para LPC4337 (Cortex-M4).
 * Proporciona base de tiempo de 1ms con protección contra optimización.
 */

#include "systick.h"

/**
 * @brief Contador global de ticks.
 * 'static' para encapsulamiento y 'volatile' para que el compilador
 * no asuma que el valor es constante dentro de los bucles while.
 */
static volatile uint32_t msTicks = 0;

/**
 * @brief Handler de la interrupción del SysTick.
 * Usamos el atributo 'used' para asegurar que el linker no lo elimine
 * si no detecta llamadas explícitas en el código.
 */
__attribute__((used))
void SysTick_Handler(void) {
    msTicks++;
}

/**
 * @brief Inicializa el SysTick con resolución de 1ms.
 * En lugar de usar valores fijos (204000), usamos la variable dinámica
 * SystemCoreClock para que el driver sea portátil y preciso.
 */
void SysTick_Init(void) {
    msTicks = 0;

    /* * SysTick_Config es la función estándar de CMSIS que:
     * 1. Calcula el valor de LOAD (SystemCoreClock / 1000).
     * 2. Configura la prioridad de la interrupción.
     * 3. Limpia el valor actual (VAL).
     * 4. Habilita el Counter, la Interrupción y usa el Clock interno.
     */
    if (SysTick_Config(SystemCoreClock / 1000)) {
        /* Error en la configuración: el valor de recarga excede los 24 bits.
         * En la EDU-CIAA, esto pasaría si el clock fuera > 16.7 GHz (imposible).
         * Nos quedamos aquí como medida de seguridad (Capa de Diagnóstico). */
        while (1); 
    }

    /* Opcional: Aseguramos que las interrupciones globales estén activas */
    __enable_irq(); 
}

/**
 * @brief Retorna el tiempo transcurrido desde el reset.
 */
uint32_t SysTick_GetTicks(void) {
    return msTicks;
}

/**
 * @brief Genera un retardo bloqueante basado en ticks.
 * Utiliza aritmética de diferencia para ser inmune al desbordamiento (rollover)
 * de la variable msTicks cada 49.7 días.
 */
void SysTick_Delay(uint32_t ms) {
    uint32_t startTicks = msTicks;
    
    /* * Mientras la diferencia sea menor al tiempo solicitado, esperamos.
     * Al ser uint32_t, la resta (msTicks - startTicks) siempre da el valor
     * absoluto correcto incluso si msTicks vuelve a cero durante la espera.
     */
    while ((msTicks - startTicks) < ms);
}