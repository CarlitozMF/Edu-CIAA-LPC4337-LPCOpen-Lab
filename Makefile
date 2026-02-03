# =============================================================================
# 🏗️ CAPA 0: CONFIGURACIÓN DEL ENTORNO LOCAL (Blindaje de Rutas)
# =============================================================================
# Obtenemos la ruta actual y normalizamos barras para evitar errores con espacios
ROOT_DIR := $(subst \,/,$(CURDIR))

# --- 1. RUTAS DEL PROYECTO ---
PROJ_NAME = 01_GPIO_SCU_Mux
PROJ_DIR  = projects/01_Basico/$(PROJ_NAME)
FW_DIR    = firmware_v3
OUT       = out

# --- 2. HERRAMIENTAS (Toolchain Local en /tools) ---
# Forzamos el uso de los binarios que descargaste en Documentos
BIN_PATH := $(ROOT_DIR)/tools/gcc-arm/bin/
CROSS    := $(BIN_PATH)arm-none-eabi-

CC      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
SIZE    := $(CROSS)size

# Configuración de OpenOCD (xPack)
OOCD_BIN := $(ROOT_DIR)/tools/openocd/bin/openocd.exe
OOCD_SCR := $(ROOT_DIR)/tools/openocd/openocd/scripts
CFG_FILE := $(ROOT_DIR)/lpc4337_new.cfg

# =============================================================================
# 🏗️ CAPA 1: CONFIGURACIÓN DE HARDWARE Y COMPILACIÓN
# =============================================================================
ARCH_FLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
DEFINES    = -DCORE_M4 -DBOARD=edu_ciaa_nxp -D__USE_LPCOPEN -DCHIP_LPC43XX

# --- INCLUDES (Capa 2: Abstracción de Hardware) ---
INCLUDES = -I$(PROJ_DIR)/Core/Inc \
           -I$(FW_DIR)/libs/lpc_open/lpc_chip_43xx/inc \
           -I$(FW_DIR)/libs/lpc_open/lpc_chip_43xx/inc/cmsis \
           -I$(FW_DIR)/libs/lpc_open/boards/edu_ciaa_nxp/inc \
           -I$(FW_DIR)/libs/cmsis_core/inc

CFLAGS  = $(ARCH_FLAGS) $(DEFINES) $(INCLUDES) -std=c99 -ggdb3 -O0

# --- LDFLAGS (Enlace y Memoria) ---
LDFLAGS = $(ARCH_FLAGS) -nostartfiles -L$(FW_DIR)/libs/lpc_open/lib -Tlink.ld \
          -Wl,-Map=$(OUT)/map.map --specs=nano.specs

# =============================================================================
# 🏗️ CAPA 2: GESTIÓN DE ARCHIVOS FUENTE
# =============================================================================
SRC = $(PROJ_DIR)/Core/Src/main.c \
      $(PROJ_DIR)/Core/Src/leds.c \
      $(PROJ_DIR)/Core/Src/startup_fix.c \
      $(wildcard $(FW_DIR)/libs/lpc_open/lpc_chip_43xx/src/*.c) \
      $(wildcard $(FW_DIR)/libs/lpc_open/boards/edu_ciaa_nxp/src/*.c)

# =============================================================================
# 🏗️ CAPA 3: REGLAS DE EJECUCIÓN (Output Limpio)
# =============================================================================
all: $(OUT)/$(PROJ_NAME).bin

$(OUT)/$(PROJ_NAME).elf: $(SRC)
	@mkdir -p $(OUT)
	@echo "🛠️  Compilando el proyecto: $(PROJ_NAME)..."
	@"$(CC)" $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "📊 Tamaño del binario:"
	@"$(SIZE)" $@

$(OUT)/$(PROJ_NAME).bin: $(OUT)/$(PROJ_NAME).elf
	@echo "📦 Generando archivo .bin..."
	@"$(OBJCOPY)" -O binary $< $@
	@echo "✅ ¡Compilación Exitosa!"

clean:
	@echo "🧹 Limpiando archivos temporales..."
	@rm -rf $(OUT)

# =============================================================================
# 🚀 CAPA 4: GRABACIÓN (Download)
# =============================================================================
download: all
	@echo "🚀 Grabando en Flash A (0x1A000000) con OpenOCD Local..."
	@"$(OOCD_BIN)" -s "$(OOCD_SCR)" -f "$(CFG_FILE)" \
		-c "init" \
		-c "halt" \
		-c "flash write_image erase $(OUT)/$(PROJ_NAME).bin 0x1A000000 bin" \
		-c "reset run" \
		-c "shutdown"

# =============================================================================
# 🏗️ CAPA 5: DEPURACIÓN (GDB + OpenOCD)
# =============================================================================
GDB := "$(CROSS)gdb"

debug: all
	@echo "🐞 Iniciando servidor de depuración y GDB..."
	@echo "Presioná CTRL+C para cerrar el servidor cuando termines."
	@"$(OOCD_BIN)" -s "$(OOCD_SCR)" -f "$(CFG_FILE)" \
		-c "init" \
		-c "halt"