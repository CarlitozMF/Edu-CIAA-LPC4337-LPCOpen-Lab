#include "chip.h"
#include <chip_lpc43xx.h>


void board_init(void) {
    SystemCoreClockUpdate();
    
    // 1. Configuración de pines (SCU) para el LED RGB
    Chip_SCU_PinMuxSet(2, 0, (SCU_MODE_INACT | SCU_MODE_FUNC4)); // Rojo
    Chip_SCU_PinMuxSet(2, 1, (SCU_MODE_INACT | SCU_MODE_FUNC4)); // Verde
    Chip_SCU_PinMuxSet(2, 2, (SCU_MODE_INACT | SCU_MODE_FUNC4)); // Azul

    // 2. Establecer como Salidas
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 5, 0);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 5, 1);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 5, 2);

    /* --- CONFIGURACIÓN LEDS AMARILLOS (Puertos varios) --- */
    Chip_SCU_PinMuxSet(2, 10, (SCU_MODE_INACT | SCU_MODE_FUNC0)); // LED 1
    Chip_SCU_PinMuxSet(2, 11, (SCU_MODE_INACT | SCU_MODE_FUNC0)); // LED 2
    Chip_SCU_PinMuxSet(2, 12, (SCU_MODE_INACT | SCU_MODE_FUNC0)); // LED 3

    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 0, 14); // LED 1
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1, 11); // LED 2
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1, 12); // LED 3

    // 3. APAGAR TODO (Lógica Negativa: High = Apagado)
    // Reemplazamos la función problemática por estas tres:
    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 5, 0);
    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 5, 1);
    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 5, 2);

    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 0, 14);
    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 1, 11);
    Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 1, 12);
}

int main(void) {
    board_init();

    while(1) {
        // ENCEDER (Low en EDU-CIAA)
        Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 5, 2);
        for(volatile int i = 0; i < 1000000; i++); // Un poco más lento para verlo bien
        
        // APAGAR (High en EDU-CIAA)
        Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 5, 2);
        for(volatile int i = 0; i < 1000000; i++);
    }
}