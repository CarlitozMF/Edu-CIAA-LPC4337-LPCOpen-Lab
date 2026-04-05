/**
 * @file startup.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Vector Table y Reset Handler para LPC4337 (Cortex-M4).
 * Define el punto de entrada del hardware y el mapa de excepciones.
 */

#include <stdint.h>

/* --- SÍMBOLOS DEL LINKER SCRIPT --- */
extern uint32_t __stack_end;
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

/* --- PROTOTIPOS DE EXCEPCIONES CORE --- */
extern int main(void);
extern void SysTick_Handler(void); // Definido en tu systick.c

void Reset_Handler(void);
void Default_Handler(void);

/* --- ALIASES WEAK --- 
 * Esto permite que si no defines un Handler (ej. HardFault), 
 * el micro caiga en el Default_Handler en lugar de resetearse.
 */
void NMI_Handler(void)          __attribute__ ((weak, alias ("Default_Handler")));
void HardFault_Handler(void)    __attribute__ ((weak, alias ("Default_Handler")));
void MemManage_Handler(void)    __attribute__ ((weak, alias ("Default_Handler")));
void BusFault_Handler(void)     __attribute__ ((weak, alias ("Default_Handler")));
void UsageFault_Handler(void)   __attribute__ ((weak, alias ("Default_Handler")));
void SVC_Handler(void)          __attribute__ ((weak, alias ("Default_Handler")));
void DebugMon_Handler(void)     __attribute__ ((weak, alias ("Default_Handler")));
void PendSV_Handler(void)       __attribute__ ((weak, alias ("Default_Handler")));

/* --- SECCIÓN CRP (Code Read Protection) --- */
__attribute__ ((section(".crp"))) const uint32_t crp_symbol = 0xFFFFFFFF;

/* --- RESET HANDLER --- */
void Reset_Handler(void) {
    // 1. Copiar .data de Flash a RAM
    uint32_t *src = &_etext;
    uint32_t *dst = &_data;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // 2. Limpiar .bss (Init a 0)
    dst = &_bss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // 3. Salto al main (SystemInit se llama dentro del main o CIAA_Board_Init)
    main();

    while (1);
}

/* --- HANDLER POR DEFECTO --- */
void Default_Handler(void) {
    while (1); // Trap para debug
}

/* --- TABLA DE VECTORES --- */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))&__stack_end, // 0: Top of Stack
    Reset_Handler,                // 1: Reset Handler
    NMI_Handler,                  // 2: NMI Handler
    HardFault_Handler,            // 3: HardFault Handler
    MemManage_Handler,            // 4: MPU Fault Handler
    BusFault_Handler,             // 5: Bus Fault Handler
    UsageFault_Handler,           // 6: Usage Fault Handler
    0, 0, 0, 0,                   // 7-10: Reservados
    SVC_Handler,                  // 11: SVCall Handler
    DebugMon_Handler,             // 12: Debug Monitor Handler
    0,                            // 13: Reservado
    PendSV_Handler,               // 14: PendSV Handler
    SysTick_Handler,              // 15: SysTick Handler (¡EL MOTOR!)
    
    /* A partir de aquí irían las IRQ de periféricos (DAC, DMA, Ethernet, etc.) */
};