#include <stdint.h>

/* --- SÍMBOLOS DEL LINKER SCRIPT --- */
// Usamos 'extern' para traer las direcciones calculadas en misc/link.ld
extern uint32_t __stack_end;
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

/* --- PROTOTIPOS --- */
extern int main(void);
void Reset_Handler(void);
void SystemInit(void) __attribute__((weak));

/* --- SECCIÓN CRP (Code Read Protection) --- */
// Obligatorio en LPC43xx para evitar bloqueos del micro
__attribute__ ((section(".crp"))) const uint32_t crp_symbol = 0xFFFFFFFF;

/* --- RESET HANDLER (El despertar del Micro) --- */
void Reset_Handler(void) {
    // 1. Copiar la sección .data desde Flash a RAM
    // Los valores iniciales de las variables globales están en Flash (_etext)
    uint32_t *src = &_etext;
    uint32_t *dst = &_data;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // 2. Limpiar la sección .bss (Variables globales sin inicializar a 0)
    dst = &_bss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // 3. Inicialización de Hardware básica (Relojes/PLL)
    if (SystemInit) {
        SystemInit();
    }

    // 4. Salto al programa principal
    main();

    // Si el main retorna, entramos en bucle infinito
    while (1);
}

/* --- TABLA DE VECTORES (Capa 1) --- */
// El atributo .isr_vector asegura que esto sea lo PRIMERO en la Flash
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))&__stack_end, // 0: Dirección inicial del Stack Pointer
    Reset_Handler,                 // 1: Dirección de inicio del código
};