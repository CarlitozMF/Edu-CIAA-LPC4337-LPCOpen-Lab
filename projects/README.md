# 📂 Proyectos y Laboratorios: Implementación de Firmware

Este directorio es el núcleo práctico del repositorio. Aquí se documenta la transición del conocimiento teórico hacia la implementación física sobre el **LPC4337**, utilizando el modelo de **3 Capas de Software** y garantizando la **Soberanía Técnica**.

## 🎯 Objetivo Principal
Desarrollar un set de drivers y aplicaciones de grado industrial que permitan dominar la complejidad de la arquitectura **Cortex-M4/M0**, priorizando la eficiencia del silicio, la robustez del código y la independencia de herramientas propietarias.

---

## 🏛️ Arquitectura de Software: Modelo de 3 Capas

Para garantizar la **Soberanía Técnica** y la portabilidad, los proyectos se estructuran bajo un modelo jerárquico que separa el silicio de la aplicación:

1. **Capa de Hardware (`hardware.h` / `hardware.c`):** Abstracción directa de registros y periféricos. Es la única capa con dependencia directa del **LPC4337**.
2. **Capa de Servicio/Drivers:** Lógica intermedia que gestiona el comportamiento del periférico (ej. drivers de display, sensores, MEF de debounce).
3. **Capa de Aplicación (`main.c` / `main.h`):** Orquestación de alto nivel. El archivo `main.c` se mantiene minimalista, delegando la complejidad técnica a las capas inferiores.

### 🚀 ¿Qué logramos con este ajuste?

* **Limpieza:** Justificamos por qué el `main.c` ahora es más corto y legible.
* **Portabilidad:** Si en el futuro se desea migrar el código a otra plataforma (ej. STM32), solo se debe reescribir la Capa 1.
* **Profesionalismo:** Aplicación de buenas prácticas de ingeniería mediante el desacoplamiento de código.

---

## 🏗️ Roadmap de Aprendizaje (Estructura de Niveles)

### 🟢 Nivel 01: Fundamentos y Gestión de GPIO
* **SCU & Digital I/O:** Configuración de la matriz de conmutación (System Control Unit), pull-ups/downs y modos de drenador abierto.
* **Lógica de Control:** Implementación de Máquinas de Estado Finitas (**MEF**) y técnicas de **Debounce No Bloqueante**.

### 🟡 Nivel 02: Periféricos de Precisión y Conectividad
Este nivel se divide en cuatro fases críticas para el dominio del hardware asíncrono y la autonomía de los periféricos:

#### **Fase 1: Timers Avanzados**
* **Modos de Operación:** Implementación de **Match & Capture**, generación de **PWM** para control de potencia y el uso del **SCTimer/PWM** (State Configurable Timer) para tareas de temporización complejas.

#### **Fase 2: Adquisición Analógica (ADC & DAC)**
* **ADC Mastery:** Configuración en todos sus modos: **Burst Mode** (muestreo continuo), **Scan Mode** y sincronización por hardware mediante Timers.
* **DAC:** Generación de señales analógicas y control de niveles de tensión para aplicaciones de audio o control.

#### **Fase 3: Conectividad y Protocolos Serie**
* **UART/USART:** Implementación con **Ring Buffers** e interrupciones para telemetría robusta.
* **I2C:** Comunicación con sensores externos (EEPROM, RTC) gestionando colisiones y estados del bus.
* **SPI:** Transferencia de datos a alta velocidad para memorias o displays gráficos.
* **CAN Bus:** Introducción a protocolos industriales, manejo de filtros y buzones de mensajes (Mailboxes).

### 🔴 Nivel 03: Arquitectura de Alto Rendimiento
* **DMA Mastery:** Transferencia masiva de datos (M2P, P2M, M2M) con **Zero CPU Load**.
* **Dual-Core Orchestration:** Multiprocesamiento asimétrico utilizando el núcleo **Cortex-M0** para tareas de E/S mientras el **M4** procesa algoritmos.
* **RTOS & DSP:** Gestión de tareas en tiempo real con **FreeRTOS** y procesamiento digital de señales.

---

## 🏛️ Estándar de Documentación de Proyectos

Cada laboratorio dentro de este directorio sigue una estructura estricta para garantizar la claridad técnica:

1.  **Título y Objetivos:** Periférico atacado y meta de aprendizaje.
2.  **Teoría de Operación:** Detalle del bloque de hardware y sus modos específicos (ej. Modo Burst en ADC).
3.  **Arquitectura del Software:** Desglose de las **3 Capas** y fragmentos de código clave.
4.  **Detalles de Robustez:** Técnicas aplicadas (Histeresis, Timers Maestro/Esclavo, Bit-shifting).
5.  **Mapeo de Hardware:** Tabla de pines y conexión física en la **EDU-CIAA**.
6.  **Referencias:** Referencias recomendadas para comprender el funcionamiento y toma de decisiones del proyecto.

---

## 📋 Tabla de Laboratorios Planeados

| Nivel | Proyecto | Periféricos / Modos Clave |
| :--- | :--- | :--- |
| **Básico** | [01_GPIO_SCU](./projects/01_GPIO) | SCU, GPIO, MEF. |

---
💻 **Desarrollo de Sistemas Embebidos Profesionales | LPC4337 - NXP**