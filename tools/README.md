# 🛠️ Infraestructura del Toolchain y Orquestación (VS Code)

Este documento describe la configuración de la **Capa 1 (Herramientas)** y la **Capa 2 (Automatización)** para el desarrollo profesional sobre la EDU-CIAA (LPC4337).

## 🎯 Objetivos de la Infraestructura
* Establecer un entorno de compilación **GCC Arm** local y portable.
* Automatizar ciclos de Build/Flash mediante atajos de teclado.
* Estabilizar la depuración JTAG/SWD eliminando el **Error de Protocolo FC**.

---

## 🏗️ Arquitectura del Entorno

```mermaid
graph TD
graph TD
    A[VS Code IDE] -->|F3 / F4 / F5 / F6| B[JSON Config: Tasks/Launch]
    B -->|Control| C[OpenOCD / GDB]
    C -->|JTAG/SWD| D[LPC4337 - Cortex M4]
```

---

### 🧱 1. Capa 1: Binarios, Drivers y Mapeo de Hardware

Para asegurar la **reproducibilidad** y la **Soberanía Técnica**, se utilizan herramientas locales y drivers específicos. Esto permite que el proyecto sea portable: basta con copiar la carpeta raíz para que funcione en cualquier estación de trabajo sin necesidad de modificar las variables de entorno del sistema (`PATH`).

* **Compilador (`tools/gcc-arm/bin/arm-none-eabi-gcc`):** Transforma el código fuente en C en instrucciones binarias optimizadas para la arquitectura **Cortex-M4** de la EDU-CIAA.
* **Depurador (`tools/openocd/bin/openocd.exe`):** Actúa como puente de comunicación (**GDB Server**) entre el IDE y la interfaz JTAG/SWD del microcontrolador.
* **Driver de Comunicación (WinUSB):** Configurado mediante **Zadig** específicamente en la **Interface 0** del chip FT2232HL. Esto permite que OpenOCD tome el control directo del hardware, eliminando errores de permisos o colisiones con drivers genéricos de Windows.
* **SVD (System View Description) (`misc/LPC43xx_43Sxx.svd`):** Diccionario XML que mapea las direcciones de memoria física a nombres de registros legibles (**SCU**, GPIO, Timers) dentro del entorno de depuración.
* **Gestor de Comandos (Bash):** Integrado mediante `bash.exe` para la ejecución de tareas. Garantiza que los comandos del **Makefile** (como `rm -rf` o `mkdir`) se ejecuten de forma nativa, evitando las limitaciones de la consola de comandos estándar de Windows.

---

### ⚙️ 2. Capa 2: Automatización (.vscode)
En este nivel orquestamos las herramientas de la Capa 1 mediante archivos de configuración de Visual Studio Code para estandarizar el flujo de trabajo.

#### 📄 `launch.json` (Configuración de Depuración)
Esta es la pieza crítica de **Robustez** del entorno. Se configuró un **Bypass de Reset** y un aislamiento del target para mitigar el error de protocolo **FC** (causado por la falta de respuesta del co-procesador Cortex-M0 durante el handshake JTAG).

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "🔗 CONECTAR A F6 (EXTERNAL)",
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "external", 
            "gdbTarget": "localhost:3333",
            "executable": "${workspaceFolder}/out/${input:projectName}/${input:projectName}.elf",
            "preLaunchTask": "🛠️ MAKE ALL",
            "overrideLaunchCommands": [
                "monitor halt",
                "monitor targets lpc4337.m4",
                "load",
                "tbreak main"
            ],
            "runToEntryPoint": "main",
            "preLaunchTask": "MAKE ALL",
            "showDevDebugOutput": "none"
        }
    ]
}
```

### 📄 tasks.json (Tareas de Build y Mantenimiento)

Se ha configurado un entorno de ejecución basado en Bash para asegurar que el Makefile procese correctamente las rutas y comandos de limpieza (rm, mkdir) independientemente de la terminal por defecto de Windows.

```json

{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "🧹 MAKE CLEAN",
            "type": "shell",
            "command": "make",
            "args": ["clean"],
            "options": {
                "cwd": "${workspaceFolder}",
                "shell": { "executable": "bash.exe", "args": ["-c"] }
            },
            "detail": "Limpia binarios y purga la carpeta out/."
        },
        {
            "label": "🛠️ MAKE ALL",
            "type": "shell",
            "command": "make",
            "args": ["all"],
            "options": {
                "cwd": "${workspaceFolder}",
                "shell": { "executable": "bash.exe", "args": ["-c"] }
            },
            "group": { "kind": "build", "isDefault": true },
            "problemMatcher": "$gcc",
            "detail": "Compilación incremental del proyecto seleccionado."
        },
        {
            "label": "🚀 MAKE FLASH",
            "type": "shell",
            "command": "make",
            "args": ["flash"],
            "options": {
                "cwd": "${workspaceFolder}",
                "shell": { "executable": "bash.exe", "args": ["-c"] }
            },
            "detail": "Graba el binario directamente en la memoria Flash."
        },
        {
            "label": "🐞 MAKE DEBUG",
            "type": "shell",
            "command": "make",
            "args": ["debug"],
            "options": {
                "cwd": "${workspaceFolder}",
                "shell": { "executable": "bash.exe", "args": ["-c"] }
            },
            "detail": "Inicia el servidor OpenOCD (GDB Server) persistente."
        }
    ]
}

```

## 🛡️ Detalles de Robustez y Troubleshooting

Para asegurar una experiencia de desarrollo estable en la **EDU-CIAA (LPC4337)** y mitigar la inestabilidad propia de su arquitectura asimétrica, se implementaron las siguientes medidas técnicas:

* **Bypass de Error FC (Dual-Core Isolation):** Se eliminó la instrucción `monitor reset halt` de los comandos de inicio en el `launch.json`. Esto evita que el JTAG intente sincronizar el núcleo **Cortex-M0** (coprocesador), cuya falta de respuesta suele disparar *timeouts* de protocolo y bloquear la sesión de GDB en el núcleo principal **M4**.
* **Arquitectura de Depuración Persistente (Modo Attach):** Se implementó una configuración de servidor externo (`servertype: external`) que se conecta al puerto **3333**. Este enfoque evita que VS Code intente lanzar instancias redundantes de OpenOCD, eliminando el error de *"GDB Server Quit Unexpectedly"* por colisión de puertos.
* **Limpieza Automática de Procesos Zombies:** En caso de bloqueo del driver FTDI o puerto ocupado, se utiliza la terminal integrada para liberar el hardware de forma forzada:

```bash
    # Ejecutar en la terminal Bash de VS Code si el puerto 3333 está tomado
    taskkill /F /IM openocd.exe
```

* **Validación de Capa 1 (Auditoría de Silicio):** La integración del archivo **SVD** permite realizar una inspección de los registros del **SCU** (System Control Unit) y **GPIO** en tiempo real. Esto permite validar físicamente el multiplexado de pines sin necesidad de instrumental externo, asegurando que la configuración del hardware coincida con el diseño de software.

---

## 🔌 Configuración de Hardware y Drivers (Capa 0)

Para que el Toolchain pueda comunicarse con la **EDU-CIAA**, es necesario asegurar la correcta enumeración del chip **FTDI (FT2232HL)**. Este chip es un conversor dual que maneja tanto la depuración (JTAG) como la comunicación serie (UART).

### 🛠️ Intervención con Zadig
Para habilitar el control total de **OpenOCD** sobre la interfaz de depuración, se debe reemplazar el driver genérico de Windows por el driver **WinUSB**:

1.  Conectar la EDU-CIAA y ejecutar **Zadig**.
2.  Activar `Options` -> `List All Devices`.
3.  Seleccionar **Dual RS232-HS (Interface 0)**.
    * *Nota:* Es vital no tocar la *Interface 1*, ya que es la encargada del puerto COM (UART).
4.  Cambiar el driver actual por **WinUSB (v6.1.7600.16385)** y presionar `Replace Driver`.

### 🏷️ Identificación del Dispositivo
En entornos de depuración profesional, es común que existan conflictos de nombres. Se ha configurado el entorno para identificar la placa mediante su descripción de hardware:
* **Device Name:** `Dual RS232-HS`
* **Configuración OpenOCD:** Se utiliza el archivo `lpc4337.cfg` que especifica el `ftdi_device_desc` exacto, evitando que el depurador intente conectarse a otros periféricos FTDI conectados a la PC.

> **💡 Tip de Estabilidad:** Si el comando `F5` o `F6` falla con el error `libusb_open() failed`, verifica en el Administrador de Dispositivos que la **Interface 0** figure bajo "Universal Serial Bus devices" como un dispositivo WinUSB.

---

## ⌨️ Mapeo de Atajos Actualizado (Productividad)

Se han configurado los siguientes *Keybindings* para optimizar el ciclo de desarrollo bajo entorno **Bash**, permitiendo una transición fluida entre la limpieza, compilación y despliegue:

| Tecla | Función | Descripción Técnica |
| :---: | :--- | :--- |
| **F3** | **Clean** | Ejecuta `make clean`. Purga la carpeta `out/` y elimina archivos temporales de compilaciones previas. |
| **F4** | **Build** | Ejecuta `make all`. Realiza una compilación incremental forzada a través de la shell de Bash. |
| **F5** | **Flash** | Ejecuta `make flash`. Realiza la grabación directa del binario en la memoria Flash sin inicializar capas de depuración. |
| **F6** | **Debug Server** | Ejecuta `make debug`. Inicia el servidor **OpenOCD** de forma persistente para permitir conexiones externas de GDB. |

---

*Un Toolchain independiente es la garantía de que el conocimiento pertenece al desarrollador y no a la herramienta."*

 💻 **Desarrollo de Sistemas Embebidos Profesionales**
