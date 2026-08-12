# =============================================================================
# 🏗️  MAKEFILE INDIVIDUAL DE PROYECTO - EDU-CIAA (LPC4337)
# =============================================================================
# Autor: Mamani Flores Carlos (UTN FRT)
# =============================================================================

# 📂 1. RUTAS RELATIVAS Y AUTO-DETECCIÓN
# -----------------------------------------------------------------------------
# Forzamos a PROJECT a tomar estrictamente la carpeta local sin usar paths absolutos
PROJECT    := $(lastword $(subst /, ,$(subst \,/,$(CURDIR))))
REPO_ROOT  := ../..

CORE_DIR   := Core
OUT        := out

LIBS_DIR   := $(REPO_ROOT)/libs
MISC_DIR   := $(REPO_ROOT)/misc
TOOLS_DIR  := $(REPO_ROOT)/tools

# 🛠️  2. TOOLCHAIN (Detección de Sistema Operativo)
# -----------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    BIN_PATH   := $(TOOLS_DIR)/gcc-arm/bin/
    OOCD       := $(TOOLS_DIR)/openocd/bin/openocd.exe
    OOCD_SCR   := $(TOOLS_DIR)/openocd/scripts
    
    RM         := rm -rf
    MKDIR      := mkdir -p
    NULL_OUT   := > /dev/null 2>&1
else
    BIN_PATH   := 
    OOCD       := openocd
    OOCD_SCR   := /usr/share/openocd/scripts
    
    RM         := rm -rf
    MKDIR      := mkdir -p
    NULL_OUT   := > /dev/null 2>&1
endif

CROSS      := $(BIN_PATH)arm-none-eabi-
CC         := $(CROSS)gcc
OBJCOPY    := $(CROSS)objcopy
SIZE       := $(CROSS)size

# 🎼 3. COMPOSICIÓN DE FUENTES
# -----------------------------------------------------------------------------
PROJ_SRC   := $(wildcard $(CORE_DIR)/Src/*.c)
STARTUP    := $(LIBS_DIR)/startup/src/startup.c
DRIVERS    := $(wildcard $(LIBS_DIR)/lpc_open/lpc_chip_43xx/src/*.c)

CUSTOM_DRV := $(wildcard $(LIBS_DIR)/custom_drivers/ciaa_board/*.c) \
              $(wildcard $(LIBS_DIR)/custom_drivers/gpio/*.c)

SRC        := $(STARTUP) $(PROJ_SRC) $(CUSTOM_DRV) $(DRIVERS)

# 🚩 4. BANDERAS DE INGENIERÍA
# -----------------------------------------------------------------------------
ARCH_FLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
DEFINES    := -DCORE_M4 -DBOARD=edu_ciaa_nxp -D__USE_LPCOPEN -DCHIP_LPC43XX

INCLUDES   := -I$(CORE_DIR)/Inc \
              -I$(LIBS_DIR)/custom_drivers/sys_core \
              -I$(LIBS_DIR)/custom_drivers/systick \
              -I$(LIBS_DIR)/custom_drivers/ciaa_board \
              -I$(LIBS_DIR)/custom_drivers/gpio \
              -I$(LIBS_DIR)/custom_drivers/led  \
              -I$(LIBS_DIR)/lpc_open/lpc_chip_43xx/inc \
              -I$(LIBS_DIR)/cmsis_core/inc

CFLAGS     := $(ARCH_FLAGS) $(DEFINES) $(INCLUDES) -std=c99 -ggdb3 -Og \
              -fno-unwind-tables -fno-asynchronous-unwind-tables \
              -MMD -MP

# 🔗 5. LDFLAGS
# -----------------------------------------------------------------------------
LDFLAGS    := $(ARCH_FLAGS) -nostartfiles -T"$(MISC_DIR)/link.ld" \
              -Wl,-Map="$(OUT)/$(PROJECT).map" --specs=nano.specs

# =============================================================================
# ⚡ REGLAS DE EJECUCIÓN
# =============================================================================

# Objetos dentro de out/obj/
OBJS := $(addprefix $(OUT)/obj/, $(notdir $(SRC:.c=.o)))
DEPS := $(OBJS:.o=.d)

# VPATH le dice a Make dónde encontrar los .c usando solo nombres de archivo
vpath %.c $(sort $(dir $(SRC)))

# Declaración explícita de archivos objetivo
ELF_TARGET := $(OUT)/$(PROJECT).elf
BIN_TARGET := $(OUT)/$(PROJECT).bin

all: $(BIN_TARGET)

$(ELF_TARGET): $(OBJS)
	@echo ""
	@echo "🔗 Enlazando Objetos: $(PROJECT)..."
	@"$(CC)" $(LDFLAGS) $^ -o "$@"
	@echo ""
	@echo "📊 Reporte de Memoria:"
	@"$(SIZE)" "$@"

$(OUT)/obj/%.o: %.c
	@$(MKDIR) "$(dir $@)"
	@echo "🔨 Compilando: $<"
	@"$(CC)" $(CFLAGS) -c $< -o "$@"

$(BIN_TARGET): $(ELF_TARGET)
	@echo ""
	@echo "📦 Empaquetando binario final..."
	@"$(OBJCOPY)" -O binary "$(ELF_TARGET)" "$@"
	@echo "✨ ¡Listo! Binario generado en: $@"
	@echo ""

clean:
	@echo ""
	@echo "🧹 Limpiando laboratorio del proyecto: $(PROJECT)..."
	@$(RM) "$(OUT)"
	@echo "✅ Limpieza finalizada."
	@echo ""

flash: all
	@echo ""
	@echo "🚀 Grabando $(PROJECT) en la EDU-CIAA..."
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg" \
		-c "init" -c "halt" \
		-c "flash write_image erase $(BIN_TARGET) 0x1A000000 bin" \
		-c "reset run" -c "shutdown" $(NULL_OUT)
	@echo "✨ ¡Grabación Exitosa! El micro se está reiniciando..."
	@echo ""

debug: all
	@echo ""
	@echo "🐞 Iniciando servidor de depuración para $(PROJECT)..."
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg"

-include $(DEPS)

.PHONY: all clean flash debug