ROOT   ?= ../..
BUILD  ?= build
TARGET ?= firmware

include $(ROOT)/ee_make/toolchain.mk
include $(ROOT)/ee_hal/arch/$(ARCH)/arch.mk

CFLAGS  += $(CPUFLAGS) -std=gnu11 -Os -g3 -Wall -Wextra \
           -ffreestanding -ffunction-sections -fdata-sections \
           -I$(ROOT)/ee_hal/include
CFLAGS  += $(foreach v,$(filter CONFIG_%,$(.VARIABLES)),\
             $(if $(filter y,$($(v))),-D$(v)=1))
LDFLAGS += $(CPUFLAGS) -nostartfiles --specs=nano.specs --specs=nosys.specs \
           -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map -T $(LDSCRIPT)

# Collect obj-y from each subdirectory (Kbuild style, no recursive make).
objs :=
define collect
obj-y :=
include $(ROOT)/$(1)/Makefile
objs += $$(addprefix $(BUILD)/$(1)/,$$(obj-y))
endef
$(foreach d,$(subdirs),$(eval $(call collect,$(d))))

all: $(BUILD)/$(TARGET).bin

$(BUILD)/$(TARGET).elf: $(objs)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/%.o: $(ROOT)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

CUBEPROG ?= STM32_Programmer_CLI
OPENOCD  ?= openocd

# Write the firmware to the flash memory with STM32CubeProgrammer.
flash: $(BUILD)/$(TARGET).elf
	$(CUBEPROG) -c $(CUBEPROG_CONNECT) -w $< -v -rst

# Write the firmware to the flash memory with OpenOCD.
openocd: $(BUILD)/$(TARGET).elf
	$(OPENOCD) -f $(OPENOCD_INTERFACE) $(OPENOCD_EXTRA) -f $(OPENOCD_TARGET) \
		$(OPENOCD_RESET) -c "program $< verify reset exit"

clean:
	rm -rf $(BUILD)

-include $(objs:.o=.d)
.PHONY: all flash openocd clean