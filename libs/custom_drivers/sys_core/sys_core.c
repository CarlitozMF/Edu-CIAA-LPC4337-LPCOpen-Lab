/**
 * @file sys_core.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Implementación de la inicialización soberana del sistema para LPC4337.
 * * Este módulo garantiza que el procesador y los periféricos base entren en un 
 * estado operativo determinístico antes de ceder el control a la aplicación.
 * * @version 1.0
 * @date 2026-04-04
 */

#include "sys_core.h"

/* Definición real de las frecuencias para LPCOpen */
const uint32_t OscRateIn = 12000000; 
const uint32_t ExtRateIn = 0;

/**
 * @brief Inicializa el tridente crítico del sistema (Reloj, Placa y Tiempo).
 * * La secuencia de inicialización es estrictamente jerárquica:
 * 1. Deshabilitación de interrupciones para evitar cambios de contexto prematuros.
 * 2. Sincronización de la frecuencia del sistema con el PLL de hardware.
 * 3. Mapeo de la Matriz de Conmutación (SCU) para la EDU-CIAA.
 * 4. Activación del metrónomo de sistema (SysTick).
 * 5. Habilitación de interrupciones y sincronización del pipeline del CPU.
 * * @note Esta función debe ser la primera instrucción ejecutada en el main().
 */
void Sys_Init(void) {
    /* 1. Bloqueo de interrupciones para garantizar una inicialización atómica y segura */
    Sys_DisableInterrupts();

    /* 2. Sincronización de Reloj: Calcula SystemCoreClock basado en el estado actual del PLL.
     * Es vital para que el cálculo de ticks del SysTick sea preciso. */
    SystemCoreClockUpdate();

    /* 3. Hardware Mapping Base: Ejecuta el muxeo de pines (SCU) para los periféricos 
     * integrados de la EDU-CIAA (LEDs y Pulsadores). */
    CIAA_Board_Init();

    /* 4. Motor de Tiempo: Inicia la interrupción periódica del System Tick con resolución de 1ms. */
    SysTick_Init();

    /* 5. Apertura de interrupciones y ejecución de barrera de sincronización (ISB).
     * La barrera asegura que el CPU limpie su pipeline y reconozca los cambios de 
     * configuración antes de continuar la ejecución. */
    Sys_EnableInterrupts();
    Sys_BarrierSync();
}