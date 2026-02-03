# 🏗️ 01_GPIO_SCU_Mux: Fundamentos de Ruteo y Control Digital

Este laboratorio marca el inicio del trabajo profesional sobre la **EDU-CIAA (LPC4337)**. El enfoque principal es comprender la arquitectura de ruteo de señales, dominando la separación física entre los **Pines del Integrado** y los **Periféricos Internos** mediante la **SCU (System Control Unit)**.

---

## 🎯 Objetivos
* Configurar el multiplexado de pines mediante el registro **SCU** para asignar funciones GPIO.
* Controlar la dirección de flujo de datos (Entrada/Salida) en el puerto **GPIO**.
* Implementar un parpadeo (Blink) mediante manipulación de registros de estado en modo híbrido (**LPCOpen**).

---

## 📖 Teoría de Operación: El SCU (System Control Unit)
A diferencia de microcontroladores más simples (como AVR o PIC), el **LPC4337** requiere un paso de configuración previo para conectar el mundo exterior con el núcleo:

1.  **Pin Configuration (SCU):** Se selecciona qué función tendrá el pin físico (GPIO, UART, PWM, etc.). En este caso, usamos la `FUNC0`.
2.  **Peripheral Configuration (GPIO):** Una vez que el pin está ruteado al periférico GPIO, se define su dirección (entrada o salida) mediante el registro `DIR`.



> **Nota Técnica:** Un error frecuente en esta arquitectura es intentar escribir en un puerto GPIO sin haber configurado previamente la función en el SCU; en tal caso, el hardware no responderá a pesar de que la lógica de software sea correcta.

---

## 📂 Arquitectura del Software
Siguiendo la metodología de **3 capas** para asegurar la portabilidad y limpieza del código:

* **Capa 1 (Hardware Abstraction):** Uso de `Chip_SCU_PinMuxSet` para el ruteo y `Chip_GPIO_SetPinDIROutput` para la dirección.
* **Capa 2 (Drivers):** (En desarrollo) Creación de funciones de abstracción para los LEDs y botones de la placa.
* **Capa 3 (Aplicación):** Lógica de control temporal (Blink) para validación inicial de hardware.

---

## 🗺️ Mapeo de Hardware
Para este laboratorio, se utiliza el **LED 3** (Amarillo) integrado en la placa:

| Componente | Pin Físico (SCU) | Puerto GPIO | Bit GPIO | Función |
| :--- | :--- | :--- | :--- | :--- |
| **LED 3** | **P2_12** | **GPIO 5** | **2** | **FUNC0** |

---

## 🛠️ Detalles de Robustez
* **Modo Inactivo:** El pin se configura con `SCU_MODE_INACT` para evitar consumos innecesarios por resistencias de pull-up/down internas si no son requeridas por la aplicación.
* **Volatile:** Se utiliza la palabra clave `volatile` en los bucles de retardo rústicos para asegurar que el optimizador del compilador **GCC** no elimine la demora durante el proceso de compilación.

---

## 🚀 Conclusión
Este laboratorio establece la base técnica para todos los proyectos siguientes en la EDU-CIAA. Dominar el SCU es fundamental para utilizar periféricos críticos como la UART, el ADC o el manejo de interrupciones externas (NVIC).

---
---
*Exploraciones técnicas y laboratorios desarrollados por iniciativa propia para dominar el silicio.*