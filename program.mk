PROGRAM = projects/01_Basico/01_GPIO_SCU_Mu# --- CAPA 1: DEFINICIÓN DE HERRAMIENTAS LOCALES ---

# Usamos $(CURDIR) para que las rutas sean relativas a la raíz del repo
OPENOCD_BIN     := "$(CURDIR)/tools/openocd/bin/openocd.exe"
OPENOCD_SCRIPTS := "$(CURDIR)/tools/openocd/openocd/scripts"
CFG_FILE        := "$(CURDIR)/lpc4337_new.cfg"

# El archivo .bin que queremos grabar (esta variable vendrá del Makefile principal)
# Si no viene definida, usamos una por defecto para evitar errores
TARGET_BIN ?= $(BIN_FILE)

# --- CAPA 2: COMANDOS DE FLASH ---

# Comando para grabar la Flash Bank A (0x1a000000)
# -s: Busca los scripts internos de OpenOCD
# -f: Carga tu configuración de la EDU-CIAA
# -c: Ejecuta la secuencia de comandos de grabado
download:
	@echo "--- Iniciando Flasheo en EDU-CIAA (LPC4337) ---"
	$(OPENOCD_BIN) -s $(OPENOCD_SCRIPTS) \
	-f $(CFG_FILE) \
	-c "init" \
	-c "reset halt" \
	-c "flash write_image erase $(TARGET_BIN) 0x1a000000" \
	-c "reset run" \
	-c "shutdown"
	@echo "--- Proceso Terminado con Éxito ---"