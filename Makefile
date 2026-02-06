# =============================================================================
# 🏗️  MAKEFILE MAESTRO - ECOSISTEMA SOBERANO EDU-CIAA (LPC4337)
# =============================================================================
# Autor: Carlitoz MF & Gemini
# Descripción: Orquestador multianidado para compilación Bare Metal ARM.
# =============================================================================

# 🚀 1. SELECCIÓN DEL PROYECTO
# Puedes cambiar el proyecto por defecto aquí o pasarlo por terminal:
# Ejemplo: make PROJECT=02_Timers download
PROJECT ?= 01_GPIO

# 📂 2. CAPA 0: BLINDAJE DE RUTAS (Estructura de Directorios)
ROOT_DIR   := $(subst \,/,$(CURDIR))
PROJ_DIR   := projects/$(PROJECT)
LIBS_DIR   := libs
MISC_DIR   := misc
OUT        := out/$(PROJECT)

# 🛠️  3. CAPA 1: TOOLCHAIN (Herramientas Locales)
# Usamos rutas relativas para garantizar la portabilidad total del laboratorio
BIN_PATH   := $(ROOT_DIR)/tools/gcc-arm/bin/
CROSS      := $(BIN_PATH)arm-none-eabi-

CC         := $(CROSS)gcc
OBJCOPY    := $(CROSS)objcopy
SIZE       := $(CROSS)size
OOCD       := $(ROOT_DIR)/tools/openocd/bin/openocd.exe
OOCD_SCR   := $(ROOT_DIR)/tools/openocd/openocd/scripts

# 🎼 4. CAPA 2: COMPOSICIÓN DE FUENTES (Orden de Enlace)
# NOTA: startup.c DEBE ir primero para asegurar la Tabla de Vectores al inicio
STARTUP    := $(LIBS_DIR)/startup/src/startup.c
PROJ_SRC   := $(wildcard $(PROJ_DIR)/Core/Src/*.c)
DRIVERS    := $(wildcard $(LIBS_DIR)/lpc_open/lpc_chip_43xx/src/*.c)
BOARD      := $(wildcard $(LIBS_DIR)/lpc_open/boards/edu_ciaa_nxp/src/*.c)

SRC        := $(STARTUP) $(PROJ_SRC) $(DRIVERS) $(BOARD)

# 🚩 5. CAPA 3: BANDERAS DE INGENIERÍA (Hard-FP & Thumb)
# -mfloat-abi=hard: Usa la unidad de punto flotante (FPU) por hardware
# -fno-unwind-tables: Evita generar tablas de excepciones que rompen el binario
ARCH_FLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
DEFINES    := -DCORE_M4 -DBOARD=edu_ciaa_nxp -D__USE_LPCOPEN -DCHIP_LPC43XX

INCLUDES   := -I$(PROJ_DIR)/Core/Inc \
              -I$(LIBS_DIR)/lpc_open/lpc_chip_43xx/inc \
              -I$(LIBS_DIR)/lpc_open/boards/edu_ciaa_nxp/inc \
              -I$(LIBS_DIR)/cmsis_core/inc

CFLAGS     := $(ARCH_FLAGS) $(DEFINES) $(INCLUDES) -std=c99 -ggdb3 -O0 \
              -fno-unwind-tables -fno-asynchronous-unwind-tables

# 🔗 6. CAPA 4: LDFLAGS (El Mapa de Memoria)
# -nostartfiles: Ignora los archivos de inicio estándar para usar nuestro startup.c
# -T: Especifica el Linker Script sagrado en misc/
LDFLAGS    := $(ARCH_FLAGS) -nostartfiles -T"$(MISC_DIR)/link.ld" \
              -Wl,-Map=$(OUT)/$(PROJECT).map --specs=nano.specs

# =============================================================================
# ⚡ REGLAS DE EJECUCIÓN
# =============================================================================

all: $(OUT)/$(PROJECT).bin

$(OUT)/$(PROJECT).elf: $(SRC)
	@mkdir -p $(OUT)
	@echo "🛠️  Compilando Proyecto Soberano: $(PROJECT)..."
	@echo "   (Fuentes: $(words $(SRC)) archivos detectados)"
	@"$(CC)" $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "📊 Reporte de Memoria:"
	@"$(SIZE)" $@

$(OUT)/$(PROJECT).bin: $(OUT)/$(PROJECT).elf
	@echo "📦 Empaquetando binario final..."
	@"$(OBJCOPY)" -O binary $< $@
	@echo "✅ ¡Listo! Binario generado en: $@"

clean:
	@echo "🧹 Limpiando laboratorio del proyecto: $(PROJECT)..."
	@rm -rf $(OUT)

# 🚀 GRABACIÓN (Flash)
flash: all
	@echo "🚀 Grabando $(PROJECT) en la EDU-CIAA..."
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg" \
		-c "init" -c "halt" \
		-c "flash write_image erase $(OUT)/$(PROJECT).bin 0x1A000000 bin" \
		-c "reset run" -c "shutdown" > NUL 2>&1
	@echo "✨ ¡Grabación Exitosa! El micro se está reiniciando..."

# 🐞 DEPURACIÓN (Debug)
# Esta regla lanza el servidor OpenOCD y lo deja a la espera de GDB.
debug: all
	@echo "🐞 Iniciando servidor de depuración para $(PROJECT)..."
	@echo "   (Esperando conexión de VS Code / GDB en puerto 3333)"
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg"

.PHONY: all clean flash debug