# =============================================================================
# 🏗️  MAKEFILE INDIVIDUAL DE PROYECTO - EDU-CIAA (LPC4337)
# =============================================================================
# Autor: Carlitoz MF (UTN FRT)
# Proyecto: 01_GPIO
# =============================================================================

# 📂 1. RUTAS RELATIVAS Y AUTO-DETECCIÓN
# -----------------------------------------------------------------------------
# Nombre del ejecutable/proyecto (Toma el nombre de la carpeta actual)
PROJECT    := $(notdir $(CURDIR))

# Ruta hacia la raíz del repositorio desde projects/XX_NAME/
REPO_ROOT  := ../..

# Subdirectorios del proyecto local
CORE_DIR   := Core
OUT        := out

# Directorios compartidos globales
LIBS_DIR   := $(REPO_ROOT)/libs
MISC_DIR   := $(REPO_ROOT)/misc
TOOLS_DIR  := $(REPO_ROOT)/tools

# 🛠️  2. TOOLCHAIN (Detección de Sistema Operativo)
# -----------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    # Configuración para Windows (Entorno Git Bash / MSYS)
    BIN_PATH   := $(TOOLS_DIR)/gcc-arm/bin/
    OOCD       := $(TOOLS_DIR)/openocd/bin/openocd.exe
    OOCD_SCR   := $(TOOLS_DIR)/openocd/scripts
    
    RM         := rm -rf
    MKDIR      := mkdir -p
    NULL_OUT   := > /dev/null 2>&1
else
    # Configuración para Linux / Mac
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

# 🎼 3. COMPOSICIÓN DE FUENTES DE ESTE PROYECTO
# -----------------------------------------------------------------------------
# Fuentes propias del proyecto
PROJ_SRC   := $(wildcard $(CORE_DIR)/Src/*.c)

# Fuentes compartidas globales
STARTUP    := $(LIBS_DIR)/startup/src/startup.c
DRIVERS    := $(wildcard $(LIBS_DIR)/lpc_open/lpc_chip_43xx/src/*.c)

# Custom Drivers específicos necesarios para ESTE proyecto
# (Aquí incluyes solo los drivers que usa este proyecto específico)
CUSTOM_DRV := $(wildcard $(LIBS_DIR)/custom_drivers/ciaa_board/*.c) \
              $(wildcard $(LIBS_DIR)/custom_drivers/gpio/*.c)

# Unificación de fuentes
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

# Banderas de compilación con soporte de dependencias automáticas (-MMD -MP)
CFLAGS     := $(ARCH_FLAGS) $(DEFINES) $(INCLUDES) -std=c99 -ggdb3 -Og \
              -fno-unwind-tables -fno-asynchronous-unwind-tables \
              -MMD -MP

# 🔗 5. LDFLAGS
# -----------------------------------------------------------------------------
LDFLAGS    := $(ARCH_FLAGS) -nostartfiles -T"$(MISC_DIR)/link.ld" \
              -Wl,-Map=$(OUT)/$(PROJECT).map --specs=nano.specs

# =============================================================================
# ⚡ REGLAS DE EJECUCIÓN
# =============================================================================

# Mapeo de objetos preservando la estructura de carpetas en out/
OBJS := $(SRC:%.c=$(OUT)/%.o)
DEPS := $(OBJS:.o=.d)

all: $(OUT)/$(PROJECT).bin

$(OUT)/$(PROJECT).elf: $(OBJS)
	@echo "🔗 Enlazando Objetos: $(PROJECT)..."
	@"$(CC)" $(LDFLAGS) $^ -o $@
	@echo "📊 Reporte de Memoria:"
	@"$(SIZE)" $@

# Regla de compilación estricta para archivos locales y externos (libs)
$(OUT)/%.o: %.c
	@$(MKDIR) $(dir $@)
	@echo "🔨 Compilando: $<"
	@"$(CC)" $(CFLAGS) -c $< -o $@

$(OUT)/$(PROJECT).bin: $(OUT)/$(PROJECT).elf
	@echo "📦 Empaquetando binario final..."
	@"$(OBJCOPY)" -O binary $< $@
	@echo "✅ ¡Listo! Binario generado en: $@"

clean:
	@echo "🧹 Limpiando binarios del proyecto: $(PROJECT)..."
	@$(RM) $(OUT)

flash: all
	@echo "🚀 Grabando $(PROJECT) en la EDU-CIAA..."
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg" \
		-c "init" -c "halt" \
		-c "flash write_image erase $(OUT)/$(PROJECT).bin 0x1A000000 bin" \
		-c "reset run" -c "shutdown" $(NULL_OUT)
	@echo "✨ ¡Grabación Exitosa! El micro se está reiniciando..."

debug: all
	@echo "🐞 Iniciando servidor de depuración para $(PROJECT)..."
	@"$(OOCD)" -s "$(OOCD_SCR)" -f "$(MISC_DIR)/lpc4337.cfg"

# Inclusión de dependencias automáticas (.h -> .d)
-include $(DEPS)

.PHONY: all clean flash debugss