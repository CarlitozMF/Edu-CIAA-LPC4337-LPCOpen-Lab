# 🛠️ Infraestructura del Toolchain

Este documento detalla la arquitectura técnica y el ecosistema de herramientas que sostienen el desarrollo sobre la **EDU-CIAA (NXP LPC4337)**. Se enfoca en la implementación de una **Capa 1 (Hardware & Tools)** y una **Capa 2 (Automatización)** robustas, preparadas para un entorno de producción.

## 🎯 1. Filosofía: Portabilidad
El diseño de este Toolchain se fundamenta en la **Portabilidad Técnica**: la capacidad del desarrollador de poseer y controlar cada etapa del ciclo de vida del software, sin depender de la opacidad de los IDEs propietarios.

* **Independencia Total**: El entorno es 100% autónomo. No requiere instalaciones globales ni configuraciones previas en el sistema operativo -Usuarios Windows-.
* **Portabilidad "Copy-Paste"**: Al utilizar binarios locales dentro del repositorio, el proyecto es agnóstico a la máquina. Basta con clonar el repositorio para que el ciclo de *Clean, Build & Debug* funcione de forma inmediata, eliminando el clásico problema de "en mi máquina funciona".
* **Transparencia y Acceso a Bajo Nivel**: El entorno no impone capas de abstracción obligatorias. Gracias a la integración de archivos **SVD** y **OpenOCD**, el desarrollador mantiene visibilidad total sobre los registros de hardware en todo momento, permitiendo trabajar tanto con bibliotecas del fabricante (**LPCOpen**) como en modo **Bare-Metal (CMSIS)** con control absoluto sobre el mapa de memoria.

---

## 📂 2. Organización del Repositorio

La estructura de carpetas está diseñada para separar estrictamente las responsabilidades del firmware, las herramientas de soporte y el mapeo de memoria, garantizando la modularidad del proyecto:

* **`libs/` (Capa 2 - Abstracción de Software)**: Es el núcleo de los drivers y middlewares del sistema. Contiene:
    * **`cmsis_core` / `cmsis_dsp`**: Estándar de interfaz de microcontroladores Cortex-M para acceso a registros del núcleo y procesamiento digital de señales.
    * **`custom_drivers`**: Controladores de autoría propia desarrollados para periféricos específicos.
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

El entorno utiliza el motor de tareas de VS Code (`tasks.json`) para orquestar el flujo de trabajo de forma agnóstica al sistema operativo. Se prioriza el uso de **Git Bash** en Windows para mantener la compatibilidad con la sintaxis del Makefile maestro.

### 🛠️ Perfiles de Automatización
El sistema detecta automáticamente el entorno de ejecución:
* **Windows**: Fuerza el uso de `bash.exe` para garantizar que comandos como `mkdir -p` y `rm -rf` se ejecuten sin errores de sintaxis de CMD.
* **Linux**: Utiliza `/bin/bash` nativo, integrándose con el toolchain instalado en el sistema.

### 🚀 Flujo de Trabajo Integrado
Se han definido tareas clave que automatizan el ciclo de vida del proyecto. Estas acciones pueden ejecutarse mediante los **botones en la Barra de Estado** de VS Code o a través de los atajos de teclado vinculados:

| Botón / Tarea | Acción Técnica | Atajo |
| :--- | :--- | :--- |
| **🛠️ BUILD** | Compilación multihilo (`-j4`) generando archivos `.elf`, `.map` y `.bin`. | `F4` |
| **⚡ FLASH** | Grabación atómica en Flash Bank A (0x1A000000) vía OpenOCD. | `F5` |
| **🗑️ CLEAN** | Limpieza total de la carpeta `out/` del proyecto seleccionado. | `F3` |
| **🐞 DEBUG** | Lanzamiento del servidor OpenOCD en segundo plano para conexión GDB. | `F6` |

> **Independencia del Desarrollador**: Gracias al uso de `inputs` dinámicos, el entorno permite cambiar de proyecto (ej. de `01_GPIO` a `02_Timers`) en tiempo real sin modificar una sola línea de configuración ni reiniciar el editor.

### 🐧 Nota de Compatibilidad para Linux
A diferencia de la arquitectura para Windows, este repositorio no incluye los binarios ejecutables para Linux para mantener la eficiencia del almacenamiento. Los usuarios de sistemas basados en Unix deben asegurar la instalación de las herramientas de grado industrial mediante el gestor de paquetes de su distribución:

#### **Debian / Ubuntu / Mint**
```bash
sudo apt update && sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi openocd make
```

#### **Arch Linux / Manjaro**
```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils openocd make
```

#### **Fedora**

```bash
sudo dnf install arm-none-eabi-gcc-cs arm-none-eabi-binutils-cs openocd make
```
#### **openSUSE**
```bash
sudo zypper install cross-arm-none-gcc12-bootstrap openocd make
```

* **Detección Automática**: El `Makefile Maestro` y el `tasks.json` detectarán automáticamente estas instalaciones globales, omitiendo las rutas de la carpeta `./tools/` y utilizando los binarios del sistema de forma transparente.
* **Permisos de Hardware**: Asegúrate de que tu usuario tenga permisos de acceso al puerto serie y al depurador JTAG (típicamente añadiendo tu usuario a los grupos `dialout`, `uucp` o `plugdev` según la distribución utilizada).

---

*"La verdadera autonomía en sistemas embebidos no proviene de un IDE propietario, sino del dominio absoluto sobre el hardware con las herramientas necesarias."*

🛠️ **Carlos** | Estudiante de Ing. Electrónica @UTN_FRT.  
🚀 Apasionado Autodidacta por los Sistemas Embebidos.