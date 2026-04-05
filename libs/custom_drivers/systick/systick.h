/**
 * @file systick.h
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Driver de Abstracción de Bajo Nivel (HAL) para el System Tick (SysTick) en LPC4337.
 * Proporciona una base de tiempo de 1ms para tareas no bloqueantes y retardos precisos.
 * @version 1.0
 * @date 2026-04-04
 */

#ifndef SYSTICK_H
#define SYSTICK_H

#include "chip.h"

/* --- Prototipos de Funciones --- */

/**
 * @brief Inicializa el SysTick para generar una interrupción cada 1ms.
 * Utiliza la frecuencia de CPU actual (SystemCoreClock).
 */
void SysTick_Init(void);

/**
 * @brief Obtiene el valor actual del contador de milisegundos.
 * @return uint32_t Ticks transcurridos desde el inicio (en ms).
 */
uint32_t SysTick_GetTicks(void);

/**
 * @brief Genera un retardo bloqueante en milisegundos.
 * @param ms Cantidad de milisegundos a esperar.
 */
void SysTick_Delay(uint32_t ms);

void SysTick_Handler(void);

#endif /* SYSTICK_H */