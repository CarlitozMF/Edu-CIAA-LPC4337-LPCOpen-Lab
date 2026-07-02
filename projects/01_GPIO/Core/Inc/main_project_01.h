#ifndef MAIN_PROJECT_01_H
#define MAIN_PROJECT_01_H

#include "hw_config_01.h"

/* =============================================================================
 * 🛠️ MACROS DE ACCIÓN (Abstracción de Software)
 * ============================================================================= */
// Macros para LED 1
#define LED1_ON()       Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF()      Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_TOGGLE()   Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED1_GPIO_PORT, LED1_GPIO_PIN)

// Macros para LED 2
#define LED2_ON()       Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_OFF()      Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_TOGGLE()   Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED2_GPIO_PORT, LED2_GPIO_PIN)

// Macros para LED 3
#define LED3_ON()       Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_OFF()      Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_TOGGLE()   Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED3_GPIO_PORT, LED3_GPIO_PIN)

// Macros para LED RGB Rojo
#define LEDR_ON()       Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, LEDR_GPIO_PORT, LEDR_GPIO_PIN)
#define LEDR_OFF()      Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, LEDR_GPIO_PORT, LEDR_GPIO_PIN)

// Macros para LED RGB Verde
#define LEDG_ON()       Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, LEDG_GPIO_PORT, LEDG_GPIO_PIN)
#define LEDG_OFF()      Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, LEDG_GPIO_PORT, LEDG_GPIO_PIN)

// Macros para LED RGB Azul
#define LEDB_ON()       Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, LEDB_GPIO_PORT, LEDB_GPIO_PIN)
#define LEDB_OFF()      Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, LEDB_GPIO_PORT, LEDB_GPIO_PIN)

//Macros para display off
#define SEG_A_OFF()     Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_A_GPIO_PORT, SEG_A_GPIO_PIN)
#define SEG_B_OFF()     Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_B_GPIO_PORT, SEG_B_GPIO_PIN)
#define SEG_C_OFF()     Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_C_GPIO_PORT, SEG_C_GPIO_PIN)
#define SEG_D_OFF()     Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_D_GPIO_PORT, SEG_D_GPIO_PIN)
#define SEG_E_OFF()     Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_E_GPIO_PORT, SEG_E_GPIO_PIN)
#define SEG_F_OFF()     Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_F_GPIO_PORT, SEG_F_GPIO_PIN)
#define SEG_G_OFF()     Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_G_GPIO_PORT, SEG_G_GPIO_PIN)
#define SEG_DP_OFF()    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, SEG_DP_GPIO_PORT, SEG_DP_GPIO_PIN)

/* --- Prototipos de funciones --- */
void board_init(void);

#endif