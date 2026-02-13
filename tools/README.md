# 🛠️ Infraestructura del Toolchain y Orquestación

Este documento detalla la arquitectura técnica y el ecosistema de herramientas que sostienen el desarrollo sobre la **EDU-CIAA (NXP LPC4337)**. Se enfoca en la implementación de una **Capa 1 (Hardware & Tools)** y una **Capa 2 (Automatización)** robustas, preparadas para un entorno de producción.

## 🎯 1. Filosofía: Soberanía Técnica y Portabilidad
El diseño de este Toolchain se fundamenta en la **Soberanía Técnica**: la capacidad del desarrollador de poseer y controlar cada etapa del ciclo de vida del software, sin depender de la opacidad de los IDEs propietarios ("Black Boxes").

* **Independencia Total**: El entorno es 100% autónomo. No requiere instalaciones globales ni configuraciones previas en el sistema operativo.
* **Portabilidad "Copy-Paste"**: Al utilizar binarios locales dentro del repositorio, el proyecto es agnóstico a la máquina. Basta con clonar el repositorio para que el ciclo de *Build & Debug* funcione de forma inmediata, eliminando el clásico problema de "en mi máquina funciona".
* **Control de Silicio**: Se prioriza la transparencia sobre los registros y la memoria, utilizando herramientas de introspección directa para validar la configuración del hardware.

---

## 📂 2. Organización del Repositorio

La estructura de carpetas está diseñada para separar estrictamente las responsabilidades del firmware, las herramientas de soporte y el mapeo de memoria, garantizando la modularidad del proyecto:

* **`libs/` (Capa 2 - Abstracción de Software)**: Es el núcleo de los drivers y middlewares del sistema. Contiene:
    * **`cmsis_core` / `cmsis_dsp`**: Estándar de interfaz de microcontroladores Cortex-M para acceso a registros del núcleo y procesamiento digital de señales.
    * **`custom_drivers`**: Controladores de autoría propia desarrollados para periféricos específicos, siguiendo la filosofía de "Soberanía Técnica".
    * **`fatfs` / `lpc_fatfs_disks`**: Sistema de archivos genérico y su capa de enlace con el hardware de la EDU-CIAA para la gestión de memorias SD.
    * **`lpc_open`**: Framework de NXP que proporciona el acceso de bajo nivel a los periféricos del LPC4337.
    * **`startup`**: Código de inicialización en ensamblador y C (Vector Table) necesario para el arranque del procesador.

* **`misc/` (Configuración de Bajo Nivel)**: Archivos críticos que definen la interacción con el silicio:
    * **link.ld**: Define el mapa de memoria física (Flash y RAM) donde se aloja el código y los datos.
    * **LPC43xx_43Sxx**: Archivo XML que permite la inspección visual de registros periféricos en tiempo real durante el debug.
    * **lpc4337.cfg**: Configuraciones de OpenOCD para el control del protocolo JTAG/SWD.

* **`projects/` (Capa 3 - Aplicación)**: Espacio dedicado a la lógica de negocio y aplicaciones de usuario (ej. `01_GPIO`).

* **`out/` (Artefactos de Compilación)**: Directorio temporal gestionado por el Makefile.
    * Contiene los archivos objeto (`.o`), el ejecutable con símbolos de depuración (`.elf`) y el binario puro para producción (`.bin`).

* **`tools/` (Capa 1 - Herramientas)**: Repositorio local de binarios que asegura la portabilidad y la independencia del sistema operativo:
    * **GCC Arm**: Compilador de grado industrial (Toolchain) para la arquitectura Cortex-M4.
    * **OpenOCD**: Servidor de depuración que actúa como puente de comunicación JTAG/SWD entre el hardware y el IDE.
    * **Make**: Motor de automatización encargado de procesar el `Makefile`. Incluirlo localmente evita conflictos de versiones y asegura que los comandos de construcción sean idénticos en cualquier entorno de desarrollo.

---

## 🎼 3. El Corazón: El Makefile (Orquestación)
El `Makefile` es el director de orquesta que automatiza la transformación del código en firmware ejecutable, garantizando que el conocimiento pertenezca al desarrollador y no a la herramienta.

* **Gestión del Toolchain**: Define rutas relativas hacia `./tools/` para asegurar la reproducibilidad exacta de la compilación.
* **Enlazado de Capas**: Coordina la unión de la Aplicación (Capa 3), los Drivers (Capa 2) y el Startup (Capa 1).
* **Targets Críticos**:
    * `make clean`: Purga la carpeta `out/` y archivos temporales.
    * `make all`: Compila el proyecto completo bajo arquitectura **Cortex-M4**.
    * `make flash`: Graba el binario directamente en la Flash.
    * `make debug`: Inicia el servidor de depuración persistente.

---

## 🔌 4. Configuración de Hardware y Drivers (Capa 0)

Para que el Toolchain pueda comunicarse con la **EDU-CIAA**, es necesario asegurar la correcta enumeración del chip **FTDI (FT2232HL)**. Este chip es un conversor dual que maneja tanto la depuración (JTAG) como la comunicación serie (UART).

### 🛠️ Intervención con Zadig
Para habilitar el control total de **OpenOCD** sobre la interfaz de depuración, se debe reemplazar el driver genérico de Windows por el driver **WinUSB**:

1.  **Conectar la EDU-CIAA** y ejecutar **Zadig**.
2.  Activar `Options` -> `List All Devices`.
3.  Seleccionar **Dual RS232-HS (Interface 0)**.
    * *Nota:* Es vital no tocar la *Interface 1*, ya que es la encargada del puerto COM (UART).
4.  Cambiar el driver actual por **WinUSB (v6.1.7600.16385)** y presionar `Replace Driver`.

### 🏷️ Identificación del Dispositivo
Se ha configurado el entorno para identificar la placa mediante su descripción de hardware exacta en el archivo `lpc4337.cfg`:
* **Device Name:** `Dual RS232-HS`
* **Bypass de Error FC:** El script `.cfg` personalizado aísla el núcleo **Cortex-M4** y omite la sincronización del núcleo **M0**, eliminando los *timeouts* de protocolo y bloqueos de GDB.

> **💡 Tip de Estabilidad:** Si el comando `F5` o `F6` falla con el error `libusb_open() failed`, verifica en el Administrador de Dispositivos que la **Interface 0** figure bajo "Universal Serial Bus devices" como un dispositivo WinUSB.

---

## ⚙️ 5. Automatización en VS Code
Para maximizar la productividad, el toolchain se integra con Visual Studio Code mediante una orquestación basada en **Bash**.

#### 📄 `launch.json` (Configuración de Depuración)
Esta es la pieza crítica de **Robustez** del entorno. Se configuró un **Bypass de Reset** y un aislamiento del target para mitigar el error de protocolo **FC** (causado por la falta de respuesta del co-procesador Cortex-M0 durante el handshake JTAG).

```json
{
    "version": "0.2.0",
    "configurations": [
        {
"name": "🐞 DEBUG SOBERANO (VIA F6)",
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "external", // <--- CAMBIO CLAVE: VS Code ya no lanza OpenOCD
            "gdbTarget": "localhost:3333", // <--- Se conecta al puente que abriste con F6
            "executable": "${workspaceFolder}/out/${input:projectName}/${input:projectName}.elf",
            "armToolchainPath": "${workspaceFolder}/tools/gcc-arm/bin",
            "cwd": "${workspaceFolder}",
            "svdFile": "${workspaceFolder}/misc/LPC43xx_43Sxx.svd",
            "runToEntryPoint": "main",
            "preLaunchTask": "🛠️ MAKE ALL", // <--- Sigue compilando antes de entrar
            "overrideLaunchCommands": [
                "monitor halt",
                "monitor targets lpc4337.m4",
                "load",
                "tbreak main"
            ],
            "showDevDebugOutput": "none"
        }
    ],
    "inputs": [
        {
            "id": "projectName",
            "type": "pickString",
            "description": "Selecciona el proyecto para el laboratorio:",
            "options": [
                "01_GPIO",
                "02_Timers",
                "03_ADC"
            ],
            "default": "01_GPIO"
        }
    ]
}
```

#### 📄 tasks.json (Tareas de Build y Mantenimiento)

Se ha configurado un entorno de ejecución basado en Bash para asegurar que el Makefile procese correctamente las rutas y comandos de limpieza (rm, mkdir) independientemente de la terminal por defecto de Windows.

```json

{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "🛠️ MAKE BUILD",
            "type": "shell",
            "command": "make",
            "args": [
                "PROJECT=${input:projectName}",
                "make" 
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": "$gcc"
        },
        {
            "label": "🚀 MAKE FLASH",
            "type": "shell",
            "command": "make",
            "args": [
                "PROJECT=${input:projectName}",
                "flash"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": "$gcc"
        },
        {
            "label": "🐞 MAKE DEBUG (Server)",
            "type": "shell",
            "command": "make",
            "args": [
                "PROJECT=${input:projectName}",
                "debug"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "isBackground": true,
            "problemMatcher": {
                "pattern": {
                    "regexp": "."
                },
                "background": {
                    "activeOnStart": true,
                    "beginsPattern": "Iniciando servidor",
                    "endsPattern": "Esperando conexión"
                }
            }
        },
        {
            "label": "🧹 MAKE CLEAN",
            "type": "shell",
            "command": "make",
            "args": [
                "PROJECT=${input:projectName}",
                "clean"
            ],
            "options": {
                "cwd": "${workspaceFolder}"
            }
        }
    ],
    "inputs": [
        {
            "id": "projectName",
            "type": "pickString",
            "description": "Selecciona el laboratorio a procesar:",
            "options": [
                "01_GPIO",
                "02_Timers",
                "03_ADC",
                "04_UART",
                "05_CAN"
            ],
            "default": "01_GPIO"
        }
    ]
}
```

### ⌨️ Mapeo de Atajos (Productividad)
| Tecla | Función | Descripción Técnica |
| :---: | :--- | :--- |
| **F3** | **Clean** | Ejecuta `make clean`. Purga binarios previos. |
| **F4** | **Build** | Ejecuta `make`. Compilación incremental vía Bash. |
| **F5** | **Flash** | Ejecuta `make flash`. Grabación directa a la Flash. |
| **F6** | **Debug Server** | Ejecuta `make debug`. Inicia OpenOCD persistente. |

---

> *"La potencia de un desarrollo no reside en la interfaz del IDE, sino en la transparencia del Toolchain."*

💻 **Desarrollo de Sistemas Embebidos Profesionales | LPC4337 - NXP**