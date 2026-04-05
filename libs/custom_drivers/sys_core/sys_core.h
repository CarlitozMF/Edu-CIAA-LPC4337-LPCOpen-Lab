/**
 * @file sys_core.h
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Driver de Abstracción de Sistema (Capa 0) para LPC4337.
 * * Este módulo centraliza la inicialización crítica del hardware (Relojes, 
 * Mapeo de Board y Base de Tiempo) y proporciona macros de control 
 * de bajo nivel para el núcleo ARM Cortex-M4.
 * * @version 1.0
 * @date 2026-04-04
 */

#ifndef SYS_CORE_H
#define SYS_CORE_H

#include "chip.h"
#include "systick.h"
#include "ciaa_board.h"

/* --- Macros de Control de Núcleo (Intrínsecas) --- */

/**
 * @brief Habilita las interrupciones globales (IRQ).
 */
#define Sys_EnableInterrupts()  __enable_irq()

/**
 * @brief Deshabilita las interrupciones globales (IRQ).
 */
#define Sys_DisableInterrupts() __disable_irq()

/**
 * @brief Barrera de Sincronización de Instrucciones (ISB).
 * Asegura que todos los cambios de contexto y configuración de sistema
 * se completen antes de ejecutar la siguiente instrucción.
 */
#define Sys_BarrierSync()       __asm__ volatile("isb")

/* --- Prototipos de Funciones --- */

/**
 * @brief Inicializa el tridente crítico del sistema.
 * * Realiza las siguientes operaciones en orden estricto:
 * 1. Actualiza la frecuencia del sistema (SystemCoreClockUpdate).
 * 2. Mapea los pines de la EDU-CIAA (CIAA_Board_Init).
 * 3. Configura la base de tiempo de 1ms (SysTick_Init).
 * * @note Esta función DEBE ser la primera en llamarse dentro del main().
 */
void Sys_Init(void);

#endif /* SYS_CORE_H */