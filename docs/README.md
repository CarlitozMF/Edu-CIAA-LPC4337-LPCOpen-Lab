# 📚 Documentación Técnica: Arquitectura de Firmware (LPC4337)

Este directorio centraliza el conocimiento técnico, los diagramas de flujo y las especificaciones de diseño del proyecto. La documentación está organizada siguiendo la **Arquitectura de 3 Capas**, facilitando la comprensión desde el silicio hasta la aplicación.

## 🛠️ Referencia de Hardware y Metodología
Antes de profundizar en los drivers, es fundamental entender cómo se mapea la **EDU-CIAA**. El siguiente manual es la base para cualquier configuración de pines:

* **[📘 Manual de Metodología de Mapeo](./00_hardware_reference/README.md):** Guía paso a paso para transformar etiquetas físicas en código funcional (SCU vs GPIO). Incluye Pinout oficial y Layout de componentes.

---

## 🏗️ Índice de Módulos y Drivers (Soberanía Técnica)

A continuación, se detallan los manuales técnicos de cada módulo desarrollado, analizando su robustez, registros y flujo de operación.

### **Nivel 0 y 1: Infraestructura y Hardware**
| Módulo | Descripción Técnica | Documentación |
| :--- | :--- | :---: |
| **SYS_CORE** | Inicialización soberana, PLL a 204MHz y barreras `ISB`. | [Link](./01-sys_core) |
| **GPIO HAL** | Gestión atómica de la SCU y registros de bits. | [Link](./03-gpio) |

### **Nivel 2: Abstracción de Periféricos y Placa**
| Módulo | Descripción Técnica | Documentación |
| :--- | :--- | :---: |
| **SysTick** | Base de tiempo de 1ms y aritmética anti-rollover. | [Link](./02-systick) |
| **CIAA_BOARD** | BSP (Board Support Package) con mapeo estático de la placa. | [Link](./04-ciaa_board) |
| **LED API** | Interfaz semántica para gestión de indicadores. | [Link](./05-led) |

---

## 📖 Fuentes de Ingeniería y Bibliografía
Para el desarrollo de estos módulos, se han consultado las siguientes fuentes oficiales:

1.  **NXP UM10503**: User Manual del LPC43xx (Capítulos 15 y 16).
2.  **Esquemático EDU-CIAA v1.1**: Referencia para el ruteo del encapsulado JBD144.
3.  **ARM Cortex-M4 TRM**: Gestión de NVIC, SysTick y pipeline de instrucciones.
4.  **LPCOpen v2.xx**: Drivers de abstracción base utilizados en la Capa 1.

---

> **Nota de Arquitectura:** Toda la documentación aquí presente busca asegurar que el firmware no sea una "caja negra", permitiendo una trazabilidad total desde el bit en el registro hasta la acción en el LED.

---

> 🛠️ Estudiante de Ing. Electrónica @UTN_FRT | Apasionado por los Sistemas Embebidos y el Low-level (ASM/C).