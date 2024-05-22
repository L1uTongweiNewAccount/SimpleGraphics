##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [4.3.0-B58] date: [Fri May 17 23:21:45 CST 2024]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = SimpleGraphics


######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -Ofast -g3 -fno-inline -fno-default-inline


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/dma.c \
Src/dma2d.c \
Src/fmc.c \
Src/ltdc.c \
Src/usart.c \
Src/stm32h7xx_it.c \
Src/stm32h7xx_hal_msp.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_fmc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_nand.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sdram.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
PicoGL/src/vertex.c \
PicoGL/src/error.c \
PicoGL/src/zmath.c \
PicoGL/src/clear.c \
PicoGL/src/api.c \
PicoGL/src/image_util.c \
PicoGL/src/get.c \
PicoGL/src/misc.c \
PicoGL/src/msghandling.c \
PicoGL/src/clip.c \
PicoGL/src/matrix.c \
PicoGL/src/light.c \
PicoGL/src/texture.c \
PicoGL/src/ztriangle.c \
PicoGL/src/select.c \
PicoGL/src/zdither.c \
PicoGL/src/zline.c \
PicoGL/src/list.c \
PicoGL/src/init.c \
PicoGL/src/oscontext.c \
PicoGL/src/zbuffer.c \
PicoGL/src/glu/glu_perspective.c \
PicoGL/src/glu/glu.c \
PicoGL/src/glu/glu_cylinder.c \
PicoGL/src/glu/glu_sphere.c \
PicoGL/src/glu/glu_disk.c \
PicoGL/src/glu/glu_torus.c \
PicoGL/src/memory.c \
PicoGL/src/arrays.c \
PicoGL/src/specbuf.c \
Src/system_stm32h7xx.c \
Src/sysmem.c \
Src/syscalls.c \
Src/User/MemoryRegion.c \
Src/User/Resolution.c \
Src/User/UserMain.c

# ASM sources
ASM_SOURCES =  \
startup_stm32h723xx.s

# ASM sources
ASMM_SOURCES = 


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
READELF = $(GCC_PATH)/$(PREFIX)readelf
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
READELF = $(PREFIX)readelf
endif
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32H723xx \
-DSTM32_THREAD_SAFE_STRATEGY=2


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32H7xx_HAL_Driver/Inc \
-IDrivers/STM32H7xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32H7xx/Include \
-IDrivers/CMSIS/Include \
-IPicoGL/include \
-IPicoGL/src \
-IPicoGL/glu \
-IPicoGL/vesafb \

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32H723ZGTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) \
  	-Wl,-Map=$(BUILD_DIR)/target/$(TARGET).map,-cref,-O,--relax,--reduce-memory-overheads

# default action: build all
all: $(BUILD_DIR)/target/$(TARGET).elf


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/object/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/object/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/object/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/object/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/object/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/object/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/target/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(CP) -O ihex -S $@ $(BUILD_DIR)/target/$(TARGET).hex
	-@rm $(BUILD_DIR)/include/symbols.h
	ctags --output-format=json --languages=c --c-kinds=+pxs --fields-c=+{macrodef} -R . \
		> $(BUILD_DIR)/target/$(TARGET).symbols.jsonl
	node scripts/GenerateSymbols.js
	$(SZ) $@
	
$(BUILD_DIR):
	mkdir $@
	mkdir $@/target
	mkdir $@/include
	mkdir $@/object

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/objects/*.d)