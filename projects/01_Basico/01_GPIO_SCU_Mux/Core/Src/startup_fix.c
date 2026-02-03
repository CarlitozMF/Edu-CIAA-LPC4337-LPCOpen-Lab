#include <stdint.h>

// El Linker Script busca ".crp", esto está bien
__attribute__ ((section(".crp"))) const uint32_t crp_symbol = 0xFFFFFFFF;

extern int main(void);
void SystemInit(void) __attribute__((weak)); 

void Reset_Handler(void) {
    if (SystemInit) SystemInit();
    main();
    while(1);
}

// CAMBIO CRÍTICO: Debe ser ".isr_vector" para que el link.ld lo vea
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))0x10008000, // 0: Top of Stack
    Reset_Handler,              // 1: Reset Handler
};