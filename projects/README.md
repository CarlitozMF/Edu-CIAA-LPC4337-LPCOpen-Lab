# 📂 Proyectos y Laboratorios: Implementación de Firmware

Este directorio es el núcleo práctico del repositorio. Aquí se documenta la transición del conocimiento teórico hacia la implementación física sobre el **LPC4337**, utilizando el modelo de **3 Capas de Software** y garantizando la **Soberanía Técnica**.

## 🎯 Objetivo Principal
Desarrollar un set de drivers y aplicaciones de grado industrial que permitan dominar la complejidad de la arquitectura **Cortex-M4/M0**, priorizando la eficiencia del silicio, la robustez del código y la independencia de herramientas propietarias.

---

## 🏛️ Arquitectura de Software: Modelo de 3 Capas

Para garantizar la portabilidad y el mantenimiento, los proyectos se estructuran bajo un modelo jerárquico que desacopla el silicio de la lógica de negocio:

1. **Capa 1: Hardware Mapping (`hw_config_0X.h`):** Es el nexo físico único por proyecto. Define matrices de estructuras `static const` con el mapeo del **SCU** y **GPIO**. Es la única capa con dependencia directa de los registros del **LPC4337**.
2. **Capa 2: Abstracción de Interfaz / Drivers (`libs/custom_drivers`):** Lógica intermedia reutilizable (ej. `led.c`, `gpio.c`). Estos drivers son agnósticos al pin físico; reciben la configuración de la Capa 1 y operan mediante **LPCOpen/CMSIS**.
3. **Capa 3: Aplicación (`main.c` / `main_project_0X.h`):** Orquestación de alto nivel y **Máquinas de Estado (MEF)**. El código es minimalista y semántico, delegando la complejidad técnica a los drivers de la Capa 2.

### 🚀 ¿Qué logramos con este diseño?
* **IntelliSense Robusto:** El uso de headers únicos (`0X`) evita colisiones de símbolos en VS Code.
* **Portabilidad:** Para migrar a otra plataforma (ej. STM32), solo se reescribe la Capa 1 y los drivers base, manteniendo la Capa 3 intacta.
* **Escalabilidad:** Permite sumar periféricos simplemente agregando elementos a los arrays de configuración.

---

## 🏗️ Roadmap de Aprendizaje (Estructura de Niveles)

### 🟢 Nivel 01: Fundamentos y Gestión de GPIO
* **SCU & Digital I/O:** Dominio de la matriz de conmutación, pull-ups/downs y modos de drenador abierto.
* **Abstracción Inicial:** Transición de macros directas a drivers basados en estructuras de datos.
* **Lógica No Bloqueante:** Implementación de **MEF** y técnicas de **Debounce** profesional sin uso de `delay()`.

### 🟡 Nivel 02: Periféricos de Precisión y Conectividad
* **Fase 1: Timers & PWM:** Match & Capture, control de potencia y uso del **SCTimer/PWM**.
* **Fase 2: Adquisición Analógica:** ADC en **Burst Mode** y **Scan Mode**, sincronización por hardware y DAC.
* **Fase 3: Protocolos Serie:** UART con **Ring Buffers**, I2C (Sensores/RTC), SPI y CAN Bus industrial.

### 🔴 Nivel 03: Arquitectura de Alto Rendimiento
* **DMA Mastery:** Transferencia masiva de datos con **Zero CPU Load**.
* **Dual-Core Orchestration:** Multiprocesamiento asimétrico (M4 + M0).
* **RTOS & DSP:** Gestión de tareas en tiempo real con **FreeRTOS** y algoritmos de filtrado.

---

## 🏛️ Estándar de Documentación de Proyectos
Cada laboratorio sigue una estructura estricta para garantizar la claridad técnica:
1. **Título y Objetivos.**
2. **Teoría de Operación (Timers Maestro/Esclavo, SCU, etc.).**
3. **Arquitectura del Software:** Diagrama Mermaid y desglose de las 3 Capas.
4. **Detalles de Robustez:** Histéresis, Bit-shifting, protección de memoria.
5. **Mapeo de Hardware:** Tabla de pines de la **EDU-CIAA**.
6. **Conclusión y Referencias.**

---

## 📋 Tabla de Laboratorios

| Nivel | Proyecto | Descripción / Periféricos Clave |
| :--- | :--- | :--- |
| **Básico** | [01_GPIO](./projects/01_GPIO) | Gestión de registros, SCU y Macros de acción. |
| **Básico** | [02_GPIO_Abstraction](./projects/02_GPIO_Abstraction) | Arquitectura de 3 Capas, Drivers HAL y Estructuras. |

---

💻 **Sistemas Embebidos | Aprendizaje Autodidacta | Carlos**