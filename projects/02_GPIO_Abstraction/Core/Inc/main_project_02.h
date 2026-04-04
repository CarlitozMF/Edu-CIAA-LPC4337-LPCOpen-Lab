/**
 * @file main.h
 * @author Carlitoz MF & Gemini
 * @brief Interfaz semántica para el Laboratorio 02 - Abstracción de GPIO.
 */

#ifndef MAIN_PROJECT_02_H
#define MAIN_PROJECT_02_H

#include "led.h"
#include "hw_config_02.h"

void RGBOFF();

/**
 * @brief Retardo simple basado en ciclos de instrucción.
 * @param count Cantidad de ciclos (proporcional a SystemCoreClock).
 */
static void delay(uint32_t count);

#endif