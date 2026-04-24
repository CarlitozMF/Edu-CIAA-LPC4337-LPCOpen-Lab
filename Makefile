# =============================================================================
# 🏗️  MAKEFILE MAESTRO - ECOSISTEMA EDU-CIAA (LPC4337)
# =============================================================================
# Autor: Carlitoz MF
# Descripción: Orquestador multianidado para compilación LPC-OPEN NXP.
# =============================================================================

# 🚀 1. SELECCIÓN DEL PROYECTO
PROJECT ?= 01_GPIO

# 📂 2. CAPA 0: BLINDAJE DE RUTAS
ROOT_DIR   := $(subst \,/,$(CURDIR))
PROJ_DIR   := projects/$(PROJECT)
LIBS_DIR   := libs
MISC_DIR   := misc
OUT        := out/$(PROJECT)

# 🛠️  3. CAPA 1: TOOLCHAIN (Detección de Sistema Operativo)
# -----------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    # CONFIGURACIÓN PARA WINDOWS
    # Usamos las herramientas locales en /tools
    BIN_PATH   := $(ROOT_DIR)/tools/gcc-arm/bin/
    OOCD       := $(ROOT_DIR)/tools/openocd/bin/openocd.exe
    OOCD_SCR   := $(ROOT_DIR)/tools/openocd/scripts
    
    # Comandos de consola (Asumiendo Bash/Git Bash en Windows)
    RM         := rm -rf
    MKDIR      := mkdir -p
    NULL_OUT   := > NUL 2>&1
else
    # CONFIGURACIÓN PARA LINUX / MAC
    # Se asume que las herramientas están en el PATH (instaladas vía sudo apt)
    BIN_PATH   := 
    OOCD       := openocd
    OOCD_SCR   := /usr/share/openocd/scripts
    
    # Comandos nativos
    RM         := rm -rf
    MKDIR      := mkdir -p
    NULL_OUT   := > /dev/null 2>&1
endif

CROSS      := $(BIN_PATH)arm-none-eabi-
CC         := $(CROSS)gcc
OBJCOPY    := $(CROSS)objcopy
SIZE       := $(CROSS)size

# 🎼 4. CAPA 2: COMPOSICIÓN DE FUENTES
STARTUP    := $(LIBS_DIR)/startup/src/startup.c
PROJ_SRC   := $(wildcard $(PROJ_DIR)/Core/Src/*.c)

ifeq ($(PROJECT), 01_GPIO)
    CUSTOM_DRV := 
else
    CUSTOM_DRV := $(wildcard $(LIBS_DIR)/custom_drivers/*/*.c)
endif

DRIVERS    := $(wildcard $(LIBS_DIR)/lpc_open/lpc_chip_43xx/src/*.c)
SRC        := $(STARTUP) $(PROJ_SRC) $(CUSTOM_DRV) $(DRIVERS)

# 🚩 5. CAPA 3: BANDERAS DE INGENIERÍA
ARCH_FLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
DEFINES    := -DCORE_M4 -DBOARD=edu_ciaa_nxp -D__USE_LPCOPEN -DCHIP_LPC43XX

INCLUDES   := -I$(PROJ_DIR)/Core/Inc \
              -I$(LIBS_DIR)/custom_drivers/sys_core \
              -I$(LIBS_DIR)/custom_drivers/systick \
              -I$(LIBS_DIR)/custom_drivers/ciaa_board \
              -I$(LIBS_DIR)/custom_drivers/gpio \
              -I$(LIBS_DIR)/custom_drivers/led  \
              -I$(LIBS_DIR)/lpc_open/lpc_chip_43xx/inc \
              -I$(LIBS_DIR)/cmsis_core/inc

CFLAGS     := $(ARCH_FLAGS) $(DEFINES) $(INCLUDES) -std=c99 -ggdb3 -Og \
              -fno-unwind-tables -fno-asynchronous-unwind-tables

# 🔗 6. CAPA 4: LDFLAGS
LDFLAGS    := $(ARCH_FLAGS) -nostartfiles -T"$(MISC_DIR)/link.ld" \
              -Wl,-Map=$(OUT)/$(PROJECT).map --specs=nano.specs

# =============================================================================
# ⚡ REGLAS DE EJECUCIÓN
# =============================================================================

OBJS := $(SRC:%.c=$(OUT)/%.o)

make: $(OUT)/$(PROJECT).bin

$(OUT)/$(PROJECT).elf: $(OBJS)
	@echo "🔗 Enlazando Objetos: $(PROJECT)..."
	@"$(CC)" $(LDFLAGS) $^ -o $@
	@echo "📊 Reporte de Memoria:"
	@"$(SIZE)" $@

$(OUT)/%.o: %.c
	@$(MKDIR) $(dir $@)
	@echo "🔨 Compilando: $<"
	@"$(CC)" $(CFLAGS) -c $< -o $@

$(OUT)/$(PROJECT).bin: $(OUT)/$(PROJECT).elf
	@echo "📦 Empaquetando binario final..."
	@"$(OBJCOPY)" -O binary $< $@
	@echo "✅ ¡Listo! Binario generado en: $@"

clean:
	@echo "🧹 Limpiando laboratorio del proyecto: $(PROJECT)..."
	@$(RM) $(OUT)

flash: make
	@echo "🚀 Grabando $(PROJECT) en la EDU-CIAA..."
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg" \
		-c "init" -c "halt" \
		-c "flash write_image erase $(OUT)/$(PROJECT).bin 0x1A000000 bin" \
		-c "reset run" -c "shutdown" $(NULL_OUT)
	@echo "✨ ¡Grabación Exitosa! El micro se está reiniciando..."

debug: make
	@echo "🐞 Iniciando servidor de depuración para $(PROJECT)..."
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg"

.PHONY: all clean flash debug