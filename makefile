################################################################################
RM := rm -rf
GNU_PREF := riscv-none-embed-
CC := $(GNU_PREF)gcc

# All of the sources participating in the build are defined here
OUTDIR := out
TARGET := $(OUTDIR)/baremetal
DEFINE :=


INC := -I .
INC += -I esp32c3
LIBS :=
#LIBS += -L. 

C_SRCS := examples/hello_world.c
C_SRCS += examples/extra.c
C_SRCS += esp32c3/startup.c

ASM_SRCS := esp32c3/boot.S
ASM_SRCS += esp32c3/vector.S

C_OBJS_TMP = $(patsubst %.c,%.o,$(C_SRCS))
C_OBJS := $(addprefix out/,$(C_OBJS_TMP))	# replace ".." to "."

LINK_SCRIPT := -T esp32c3/esp32c3.ld
LINK_SCRIPT += -T esp32c3/romfuncs.ld
LINK_SCRIPT += -T esp32c3/common.ld

ASM_OBJS_TMP += $(patsubst %.S,%.o,$(ASM_SRCS))
ASM_OBJS := $(addprefix out/,$(ASM_OBJS_TMP))	# replace ".." to "."

OBJS := $(C_OBJS) $(ASM_OBJS)

C_DEPS := $(subst .o,.d,$(C_OBJS))
ASM_DEPS := $(subst .o,.d,$(ASM_OBJS))
DEPS := $(C_DEPS) $(ASM_DEPS)

C_FLAGS := -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore 
C_FLAGS += -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections
C_FLAGS += -O2 -g -std=gnu99 -Wall -nostdlib -nostartfiles -Wno-comment $(DEFINE) 
#C_FLAGS += -save-temps  # 중간파일 안지운다.
#C_FLAGS += -DDEBUG=1

LD_FLAGS := -g $(LINK_SCRIPT) -nostdlib -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage

TARGET_FILES := $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst $(TARGET).map

DIR_GUARD = @mkdir -p $(@D)

# All Target
all:  $(TARGET_FILES) $(TARGET).size

# Each subdirectory must supply rules for building sources it contributes
$(OUTDIR)/examples/%.o: examples/%.c
	@echo "CC:" $<
	$(DIR_GUARD)
	$(CC) $(C_FLAGS) $(INC) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@

$(OUTDIR)/esp32c3/%.o: esp32c3/%.c
	@echo "CC:" $<
	$(DIR_GUARD)
	$(CC) $(C_FLAGS) $(INC) -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@


$(OUTDIR)/esp32c3/%.o: esp32c3/%.S
	@echo "ASM:" $<
	$(DIR_GUARD)
	@$(CC) $(C_FLAGS) -x assembler -MMD -MP -MF$(@:%.o=%.d) -MT$(@) -c $< -o $@ 

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# Tool invocations

$(TARGET).elf: $(OBJS)
	@echo "Making:" $@
	@$(GNU_PREF)gcc $(LD_FLAGS) -Wl,--cref,-Map,$(TARGET).map $(OBJS) -o $@  $(LIBS)

$(TARGET).hex: $(TARGET).elf
	@echo "Making:" $@
	@$(GNU_PREF)objcopy -O ihex $< $@

$(TARGET).bin: $(TARGET).elf
	@echo "Making:" $@
	@$(GNU_PREF)objcopy -O binary $< $@

$(TARGET).lst: $(TARGET).elf
	@echo "Making:" $@
	@$(GNU_PREF)objdump -d --source --all-headers --demangle --line-numbers --wide $< > $@

$(TARGET).size: $(TARGET).elf
	@$(GNU_PREF)size --format=berkeley $<

# Other Targets
clean:
	@echo Removing all output files
	@$(RM) $(TARGET_FILES)
ifneq (,$(wildcard $(OUTDIR)/*))
	@$(RM) $(OUTDIR)/*
endif

rebuild: clean all
