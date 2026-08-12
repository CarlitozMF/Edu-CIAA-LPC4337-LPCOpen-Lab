/**
 * @file main.c
 * @author Carlos Mamani Flores (UTN-FRT)
 * @brief Proyecto 03: Contador Industrial con MEF y Periféricos Externos.
 */

#include "main.h"
#include "sys_core.h"

/* --- Variables de Control de Aplicación --- */
static state_t currentState = STATE_IDLE;
static uint8_t currentCount = 0;
static uint32_t lastCountTick = 0;

/* --- Instancias de estado para cada botón --- */
static button_state_t btn_start_state = {0, true, true};
static button_state_t btn_reset_state = {0, true, true};

/* --- Variables el Blink --- */
static uint32_t lastBlinkTick = 0;
static bool displayVisible = true;

int main(void)
{
    /* 1. Inicialización Capa 0 y 2 */
    Sys_Init();

    /* 2. Configuración de Periféricos Externos (Capa 1) */
    // Inicializamos el arreglo de pines del display (definido en hardware.h)
    for (int i = 0; i < 8; i++)
    {
        GPIO_Init(&display_pins[i], GPIO_OUTPUT);
    }

    // Inicializamos pulsadores con Pull-Up (btn_start y btn_reset)
    GPIO_Init(&btn_start, GPIO_INPUT_PULLUP);
    GPIO_Init(&btn_reset, GPIO_INPUT_PULLUP);

    /* 3. Estado Inicial de Salida */
    App_DisplayOutput(0);

    /* 4. Super Loop (Capa 3) */
    while (1)
    {
        App_UpdateMEF();
    }

    return 0;
}

/**
 * @brief Implementación de la Salida: Traduce el número a los pines físicos.
 */
void App_DisplayOutput(uint8_t number)
{
    if (number > 9)
        return; // Protección de rango

    uint8_t pattern = SEGMENT_MAP[number];

    /* Recorremos los 8 pines (A-DP) y seteamos según el patrón de bits */
    for (int i = 0; i < 8; i++)
    {
        gpio_state_t state = (pattern & (1 << i)) ? GPIO_HIGH : GPIO_LOW;
        GPIO_Write(&display_pins[i], state);
    }
}

/**
 * @brief Lee un pulsador con antirrebote usando un puntero de estado.
 * @param btn Configuración del hardware.
 * @param state Puntero a la memoria específica de este botón.
 */
bool App_ReadButtonDebounced(const gpio_config_t *btn, button_state_t *state)
{
    bool currentPhysicalState = GPIO_Read(btn);
    uint32_t currentTick = SysTick_GetTicks();

    if (currentPhysicalState != state->lastPhysicalState)
    {
        state->lastCheckTick = currentTick;
    }

    if ((currentTick - state->lastCheckTick) > DEBOUNCE_DELAY)
    {
        if (currentPhysicalState != state->lastSteadyState)
        {
            state->lastSteadyState = currentPhysicalState;

            if (state->lastSteadyState == GPIO_LOW)
            {
                state->lastPhysicalState = currentPhysicalState;
                return true;
            }
        }
    }

    state->lastPhysicalState = currentPhysicalState;
    return false;
}

/**
 * @brief Apaga todos los segmentos del display.
 */
void App_DisplayClear(void) {
    for (int i = 0; i < 8; i++) {
        GPIO_Write(&display_pins[i], GPIO_LOW); // Cátodo común: LOW apaga
    }
}

/**
 * @brief Máquina de Estados Finitos.
 */
void App_UpdateMEF(void) {
    uint32_t currentTick = SysTick_GetTicks();

    // 1. Muestreo rápido: Sacamos una "foto" del estado de los botones
    // Cada uno usa su propia estructura de estado para no interferirse.
    bool resetPressed = App_ReadButtonDebounced(&btn_reset, &btn_reset_state);
    bool startPressed = App_ReadButtonDebounced(&btn_start, &btn_start_state);

    // 2. Lógica de Transiciones: Decidimos qué estado sigue
    if (resetPressed) {
        currentState = STATE_RESET;
    } 
    else if (startPressed) {
        if (currentState == STATE_IDLE || currentState == STATE_PAUSED) {
            currentState = STATE_COUNTING;
            lastCountTick = currentTick; // Sincronizamos el inicio del conteo
        } 
        else if (currentState == STATE_COUNTING) {
            currentState = STATE_PAUSED;
            App_DisplayOutput(currentCount); // Escribimos UNA VEZ al pausar
        }
    }

    // 3. Lógica de Estados: Ejecutamos lo que corresponde a cada estado
    switch (currentState) {
        case STATE_IDLE:
            displayVisible = true;
            App_DisplayOutput(currentCount);
            break;

        case STATE_COUNTING:
            displayVisible = true;
            // Incremento determinístico cada 1 segundo
            if ((currentTick - lastCountTick) >= COUNT_INTERVAL) {
                lastCountTick = currentTick;
                currentCount++;
                if (currentCount > 9) currentCount = 0;
                
                App_DisplayOutput(currentCount); // Escribimos solo porque cambió el valor
            }
            break;

        case STATE_PAUSED:
            if ((currentTick - lastBlinkTick) >= BLINK_INTERVAL) {
                lastBlinkTick = currentTick;
                displayVisible = !displayVisible; // Invertimos el estado (Toggle)

                if (displayVisible) {
                    App_DisplayOutput(currentCount); // Muestra el número
                } else {
                    // "Apaga" el display enviando 0 a todos los segmentos
                    // Si tu App_DisplayOutput no maneja esto, podés crear App_ClearDisplay()
                    App_DisplayClear(); 
                }
            }
            break;

        case STATE_RESET:
            currentCount = 0;
            displayVisible = true;
            App_DisplayOutput(currentCount); // Limpiamos el display una vez
            currentState = STATE_IDLE;       // Volvemos a esperar
            break;
            
        default:
            currentState = STATE_IDLE;
            break;
    }
}