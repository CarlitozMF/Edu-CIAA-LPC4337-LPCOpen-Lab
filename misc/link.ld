/* =============================================================================
   MAPA DE MEMORIA SOBERANO - LPC4337 (EDU-CIAA)
   Inspirado en Firmware v3 - Optimizado para Bare Metal
   ============================================================================= */

MEMORY
{
  MFlashA512 (rx) : ORIGIN = 0x1A000000, LENGTH = 0x80000 /* 512K Flash A */
  RamLoc32   (rwx): ORIGIN = 0x10000000, LENGTH = 0x8000  /* 32K RAM Principal */
  RamLoc40   (rwx): ORIGIN = 0x10080000, LENGTH = 0xA000  /* 40K RAM Secundaria */
}

/* El Stack termina al final de la RAM principal */
__stack_end = 0x10000000 + 0x8000;

ENTRY(Reset_Handler)

SECTIONS
{
    /* SECCIÓN DE TEXTO (FLASH) */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = . ;
        KEEP(*(.isr_vector))     /* Tabla de vectores */
        
        /* Code Read Protection (CRP) en la dirección 0x2FC */
        . = 0x2FC ;
        KEEP(*(.crp))
        
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .; 
    } > MFlashA512

    /* SECCIÓN DE DATOS (RAM) */
    .data : AT (_etext)
    {
        _data = .;
        *(vtable)
        *(.ramfunc*)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RamLoc32

    /* SECCIÓN BSS (Variables a cero) */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RamLoc32

    /* DESCARTES PARA EVITAR OVERLAP */
    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.ARM.extab*)
    }
}