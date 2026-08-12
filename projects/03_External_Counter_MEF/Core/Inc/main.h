/**
 * @file main_project_03.h
 * @brief Definiciones de la MEF y Lógica de Aplicación para el Contador Externo.
 */

#ifndef MAIN_PROJECT_03_H
#define MAIN_PROJECT_03_H

#include "hardware.h"

/* --- Tiempos de Sistema (ms) --- */
#define DEBOUNCE_DELAY    20    // Tiempo para filtrar rebotes mecánicos
#define COUNT_INTERVAL    1000  // Intervalo de incremento del contador (1 segundo)
#define BLINK_INTERVAL    500  // Frecuencia del parpadeo en milisegundos

/* --- Tabla de Segmentos (Cátodo Común) --- */
/* Mapea el dígito 0-9 al estado de los pines A, B, C, D, E, F, G */
static const uint8_t SEGMENT_MAP[] = {
    0x3F, // 0: 0011 1111
    0x06, // 1: 0000 0110
    0x5B, // 2: 0101 1011
    0x4F, // 3: 0100 1111
    0x66, // 4: 0110 0110
    0x6D, // 5: 0110 1101
    0x7D, // 6: 0111 1101
    0x07, // 7: 0000 0111
    0x7F, // 8: 0111 1111
    0x6F  // 9: 0110 1111
};

/* --- Definición de Estados de la MEF --- */
typedef enum {
    STATE_IDLE,      // Esperando inicio (Display en 0)
    STATE_COUNTING,  // Incrementando cada segundo
    STATE_PAUSED,    // Conteo detenido (mantiene valor)
    STATE_RESET      // Volviendo a cero
} state_t;

typedef struct {
    uint32_t lastCheckTick;
    bool lastSteadyState;
    bool lastPhysicalState;
} button_state_t;

/* --- Prototipos de Funciones de Aplicación --- */
/**
 * @brief Actualiza la lógica de la Máquina de Estados.
 */
void App_UpdateMEF(void);

/**
 * @brief Escribe un número en el display de 7 segmentos.
 * @param number Valor de 0 a 9.
 */
void App_DisplayOutput(uint8_t number);

/**
 * @brief Lee el estado de un pulsador con antirrebote no bloqueante.
 * @param btn Estructura de configuración del pin.
 * @return true si el botón está presionado.
 */
bool App_ReadButtonDebounced(const gpio_config_t *btn, button_state_t *state);

#endif