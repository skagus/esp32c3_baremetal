
################ Top configuration.
PRJ_TOP = ..
OBJDIR = out
TARGET = $(OBJDIR)/baremetal

MCU = rv32imac_zicsr_zba_zbb_zbc_zbs
#MCU = rv32imac_zicsr_zba_zbb_zbc_zbs  # B extension isn't support.
#MCU =rv64imc

BUILD_STRICT = TRUE
BUILD_PRINT = FALSE

OPTI = -O0
VER_STRING = $$(date_time +%Y%m%d_%H%M%S)_$(OPTI)

LINK_SCRIPT = esp32c3/sections.ld \
			esp32c3/rom_ref.ld

################  Define
DEFINE = \
	__riscv__

#	WCH_INT

################  Library directory.
PRJ_LIB_DIR = driver

EXT_LIB_DIR = 

LIB_FILE = c_nano

################  Include.
# Add relative path from $(PRJ_TOP)
PRJ_INC = .\
		build \
		esp32c3 \
		examples

# Add absolue path. (ex. c:/lib/inc)
EXT_INC =

################ source files ##############
# Source file들은 project TOP 에서의 위치를 나타낸다.
CSRC =	\
		esp32c3/startup.c \
		examples/extra.c \
		examples/hello_world.c

CPPSRC =

ASRC =	esp32c3/boot.S \
		esp32c3/vector.S

DATA =

include rules.mk

prog: $(TARGET).elf
	esptool --chip esp32c3 elf2image --flash_mode dio --flash_size 4MB --flash_freq 40m $(TARGET).elf
	esptool --chip esp32c3 -p COM9 write_flash 0 $(TARGET).bin
