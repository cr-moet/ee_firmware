CPUFLAGS := -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard
LDSCRIPT := $(ROOT)/ee_hal/arch/stm32h7/stm32h743zi.ld
OPENOCD_TARGET := target/stm32h7x.cfg