/**
 * @file led.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Implementación de la API de gestión de arreglos de LEDs.
 * * Este módulo actúa como una capa de abstracción superior (Capa 2.5),
 * permitiendo tratar grupos de pines GPIO como un conjunto indexado de indicadores.
 */

#include "led.h"

/**
 * @brief Inicializa un arreglo de pines como salidas para LEDs.
 * * Recorre el arreglo de configuraciones y establece cada pin como
 * salida digital con un estado inicial seguro (apagado).
 */
void LED_Init_Array(const gpio_config_t *table, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        /* Reutilizamos la lógica de inicialización de nuestra HAL de GPIO */
        GPIO_Init(&table[i], GPIO_OUTPUT);
    }
}

/**
 * @brief Establece el estado de un LED por su índice en el arreglo.
 * * @param table Puntero al arreglo de configuraciones.
 * @param index Índice del LED (debe ser menor al tamaño del arreglo).
 * @param state Estado lógico (GPIO_HIGH / GPIO_LOW).
 */
void LED_Set(const gpio_config_t *table, uint8_t index, gpio_state_t state) {
    /* * Nota de Ingeniería: Aquí la validación de 'index' dependería de 
     * conocer el tamaño del arreglo. En esta implementación, confiamos
     * en que la lógica de aplicación respeta los límites del enum.
     */
    GPIO_Write(&table[index], state);
}

/**
 * @brief Alterna el estado de un LED específico mediante su índice.
 */
void LED_Toggle(const gpio_config_t *table, uint8_t index) {
    GPIO_Toggle(&table[index]);
}