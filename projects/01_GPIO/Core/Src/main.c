/**
 * @file main.c
 * @author Carlos Mamani Flores (CarlitozMF)
 * @brief Laboratorio 01: Gestión de SCU y GPIO en arquitectura Dual-Core LPC4337.
 */

#include "chip.h"
#include "main_project_01.h"

/* Definición de frecuencias de reloj externas para la EDU-CIAA */
/* Estos valores los requiere LPCOpen (chip_43xx) para calcular los PLLs */

const uint32_t OscRateIn = 12000000;  /* Cristal principal de 12MHz */
const uint32_t ExtRateIn = 0;         /* No usamos entrada de reloj externa (ENET) */

/**
 * @brief Configuración inicial del hardware.
 * Utiliza las definiciones de hardware.h y las macros de main.h para el ruteo.
 */
void board_init(void) {
    /* Actualiza la frecuencia de clock del sistema */
    SystemCoreClockUpdate();
    
    /* --- CONFIGURACIÓN LED RGB (Capa 1: Hardware Mapping) --- */
    Chip_SCU_PinMuxSet(LEDR_SCU_PORT, LEDR_SCU_PIN, (SCU_MODE_INACT | LEDRGB_FUNC));
    Chip_SCU_PinMuxSet(LEDG_SCU_PORT, LEDG_SCU_PIN, (SCU_MODE_INACT | LEDRGB_FUNC));
    Chip_SCU_PinMuxSet(LEDB_SCU_PORT, LEDB_SCU_PIN, (SCU_MODE_INACT | LEDRGB_FUNC));

    /* Configura dirección como SALIDA (Capa 2: Abstracción) */
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LEDR_GPIO_PORT, LEDR_GPIO_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LEDG_GPIO_PORT, LEDG_GPIO_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LEDB_GPIO_PORT, LEDB_GPIO_PIN);

    /* --- CONFIGURACIÓN LEDS AMARILLOS --- */
    Chip_SCU_PinMuxSet(LED1_SCU_PORT, LED1_SCU_PIN, (SCU_MODE_INACT | LED_FUNC));
    Chip_SCU_PinMuxSet(LED2_SCU_PORT, LED2_SCU_PIN, (SCU_MODE_INACT | LED_FUNC));
    Chip_SCU_PinMuxSet(LED3_SCU_PORT, LED3_SCU_PIN, (SCU_MODE_INACT | LED_FUNC));

    /* Configura dirección como SALIDA */
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED1_GPIO_PORT, LED1_GPIO_PIN); 
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED2_GPIO_PORT, LED2_GPIO_PIN); 
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, LED3_GPIO_PORT, LED3_GPIO_PIN); 

    /* Estado Inicial: Todo apagado */
    LEDR_OFF();
    LEDG_OFF();
    LEDB_OFF();
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
}

/** * @brief Retardo por software con protección contra optimización.
 */
static void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

int main(void) {
    /* Inicialización soberana */
    board_init();

    while(1) {
        // Ejemplo de parpadeo usando las macros de main.h
        LEDB_ON();
        delay(1000000); 
        
        LEDB_OFF();
        delay(1000000);
    }

    return 0;
}

/**
 * @brief Handler de la interrupción del SysTick.
 * Se deja vacío para el Proyecto 01 ya que no utilizamos base de tiempo por interrupción.
 */
void SysTick_Handler(void) {
    // No hace nada, solo satisface al Linker.
}