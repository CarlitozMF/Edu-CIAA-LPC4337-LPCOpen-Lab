# 🏗️ Nivel Básico: Arquitectura y Control de Registros (LPC4337)

Este nivel marca el inicio del dominio sobre la **EDU-CIAA**. El enfoque principal es la transición desde el desarrollo basado en librerías de alto nivel (como sAPI) hacia un enfoque **Híbrido y Bare-Metal**, centrado en la comprensión profunda del ruteo de señales y la manipulación de registros.

---

## 🛠️ Entorno de Desarrollo
* **Hardware:** EDU-CIAA (Dual-Core Cortex-M4/M0 @ 204MHz).
* **IDE:** VS Code + GNU Arm Embedded Toolchain.
* **Estructura:** Arquitectura de 3 capas (Estilo STM32CubeIDE).
* **Concepto Clave:** Gestión del SCU (System Control Unit) y mapeo de periféricos.

---

## 📂 Laboratorios de Fundamentación

1. **[01_GPIO_SCU_Mux](./01_GPIO_SCU_Mux):** El primer contacto con el silicio. Aprendizaje del multiplexado de pines (SCU) para asignar funciones GPIO y control de salidas digitales (LEDs).
2. **[02_Digital_Inputs_Polling](./02_Digital_Inputs_Polling):** Lectura de pulsadores (TEC1-TEC4). Configuración de resistencias internas (Pull-up/Pull-down) y análisis del fenómeno de rebote (Bounce) por escaneo.
3. **[03_Interrupts_NVIC](./03_Interrupts_NVIC):** Gestión de eventos asíncronos. Introducción al controlador de interrupciones (NVIC) y prioridades para una respuesta inmediata al hardware.
4. **[04_Timers_RIT](./04_Timers_RIT):** Temporización precisa. Uso del *Repetitive Interrupt Timer* (RIT) para generar eventos periódicos sin bloquear la ejecución del CPU.
5. **[05_FSM_Debounce](./05_FSM_Debounce):** Arquitectura de software. Implementación de Máquinas de Estado Finito (MEF) para gestionar entradas digitales de forma robusta y profesional.

---

## 🚀 Proyectos Integradores (Nivel I)
*Desafíos que consolidan los conceptos básicos en aplicaciones reales.*

* **Semaforo_Inteligente:** Control de secuencias temporizadas y detección de peatones mediante interrupciones.
* **Teclado_Controlador:** Implementación de un driver de teclado matricial utilizando el SCU y lógica de escaneo por MEF.

---

## 🔬 Pilares Técnicos de este Nivel
Para cada laboratorio en este nivel, se aplica el rigor técnico de la **Capa de Abstracción de Hardware**:
* **Capa 1:** Configuración directa de registros mediante **LPCOpen / CMSIS**.
* **Capa 2:** Modularización en archivos `Core/Inc` y `Core/Src` para crear drivers reutilizables (Ej: `leds.h`, `buttons.h`).
* **Capa 3:** Lógica de aplicación agnóstica al hardware.

---
*Exploraciones técnicas y laboratorios desarrollados por iniciativa propia para dominar el silicio.*