#ifndef HW_CONFIG_01_H
#define HW_CONFIG_01_H

#include "chip.h"

/* =============================================================================
 * 📍 MAPEO FÍSICO (Basado en el User Manual UM10503)
 * ============================================================================= */

/* --- LEDs Externos --- */

/* --- LED 1 --- */
//Pin (P2_10)
#define LED1_SCU_PORT   2
#define LED1_SCU_PIN    10
//GPIO 0[14]
#define LED1_GPIO_PORT  0
#define LED1_GPIO_PIN   14

/* --- LED 2 --- */
//Pin (P2_11)
#define LED2_SCU_PORT   2
#define LED2_SCU_PIN    11
//GPIO 1[11]
#define LED2_GPIO_PORT  1
#define LED2_GPIO_PIN   11

/* --- LED 3 --- */
//Pin (P2_12)
#define LED3_SCU_PORT   2
#define LED3_SCU_PIN    12
//GPIO 1[12]
#define LED3_GPIO_PORT  1
#define LED3_GPIO_PIN   12

/* --- LEDs RGB --- */

/* --- LED R --- */
//Pin (P2_0)
#define LEDR_SCU_PORT   2
#define LEDR_SCU_PIN    0
//GPIO 5[0]
#define LEDR_GPIO_PORT  5
#define LEDR_GPIO_PIN   0

/* --- LED G --- */
//Pin (P2_1)
#define LEDG_SCU_PORT   2
#define LEDG_SCU_PIN    1
//GPIO 5[1]
#define LEDG_GPIO_PORT  5
#define LEDG_GPIO_PIN   1

/* --- LED B --- */
//Pin (P2_2)
#define LEDB_SCU_PORT   2
#define LEDB_SCU_PIN    2
//GPIO 5[2]
#define LEDB_GPIO_PORT  5
#define LEDB_GPIO_PIN   2

/*Display*/
#define SEG_A_SCU_PORT  6
#define SEG_A_SCU_PIN   1
#define SEG_A_GPIO_PORT 3
#define SEG_A_GPIO_PIN  0

#define SEG_B_SCU_PORT  6
#define SEG_B_SCU_PIN   5
#define SEG_B_GPIO_PORT 3
#define SEG_B_GPIO_PIN  4

#define SEG_C_SCU_PORT  6
#define SEG_C_SCU_PIN   8
#define SEG_C_GPIO_PORT 5
#define SEG_C_GPIO_PIN  16

#define SEG_D_SCU_PORT  6
#define SEG_D_SCU_PIN   10
#define SEG_D_GPIO_PORT 3
#define SEG_D_GPIO_PIN  6

#define SEG_E_SCU_PORT  6
#define SEG_E_SCU_PIN   4
#define SEG_E_GPIO_PORT 3
#define SEG_E_GPIO_PIN  3

#define SEG_F_SCU_PORT  6
#define SEG_F_SCU_PIN   7
#define SEG_F_GPIO_PORT 5
#define SEG_F_GPIO_PIN  15

#define SEG_G_SCU_PORT  6
#define SEG_G_SCU_PIN   9
#define SEG_G_GPIO_PORT 3
#define SEG_G_GPIO_PIN  5

#define SEG_DP_SCU_PORT  6
#define SEG_DP_SCU_PIN   11
#define SEG_DP_GPIO_PORT 3
#define SEG_DP_GPIO_PIN  7


/* --- Configuración de Modos del SCU --- */
//según pinout de la educiia
#define LED_FUNC        SCU_MODE_FUNC0  // Función GPIO para LEDs Externos
#define LEDRGB_FUNC     SCU_MODE_FUNC4  // Función GPIO para LED RGB

#endif /* HW_CONFIG_01_H */