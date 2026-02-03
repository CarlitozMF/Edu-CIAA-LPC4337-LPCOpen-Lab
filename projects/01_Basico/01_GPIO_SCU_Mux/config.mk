# Nombre del proyecto (Binario de salida)
PROJ_NAME := 01_GPIO_SCU_Mux

# Librerías a incluir (Modo Híbrido)
# lpc_open es la Capa 0 que contiene los registros del LPC4337
CP_LIBS += lpc_open

# Directorios de búsqueda de archivos de cabecera (.h)
# Añadimos Core/Inc para que el compilador encuentre leds.h
INC_DIRS += $(PROJ_PATH)/Core/Inc