/**
 * @file sys_core.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Implementación de la inicialización soberana del sistema.
 */

#include "sys_core.h"

void Sys_Init(void) {
    /* 1. Bloqueamos interrupciones para una inicialización atómica */
    Sys_DisableInterrupts();

    /* 2. Sincronización de Reloj: Calcula SystemCoreClock basado en el PLL actual */
    SystemCoreClockUpdate();

    /* 3. Hardware Mapping Base: Configura SCU para LEDs y Teclas de la EDU-CIAA */
    CIAA_Board_Init();

    /* 4. Motor de Tiempo: Inicia interrupción periódica de 1ms */
    SysTick_Init();

    /* 5. Abrimos el paso a las interrupciones y sincronizamos el pipeline del CPU */
    Sys_EnableInterrupts();
    Sys_BarrierSync();
}