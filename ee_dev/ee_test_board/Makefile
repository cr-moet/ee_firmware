ARCH    := stm32h7
TARGET  := ee_test_board

# STM32CubeProgrammer: Use SWD. Connect under reset.
CUBEPROG_CONNECT  := port=SWD mode=UR reset=HWrst

# OpenOCD
OPENOCD_INTERFACE := interface/stlink.cfg
OPENOCD_EXTRA     := -c "transport select swd"
OPENOCD_RESET     := -c "reset_config srst_only srst_nogate connect_assert_srst"

CONFIG_GPIOLIB      := y
CONFIG_GPIO_STM32H7 := y
CONFIG_SYSTICK      := y

CFLAGS  += -Iinc

subdirs := ee_hal/arch/$(ARCH) \
           ee_hal/core \
           ee_hal/drivers/gpio \
           ee_hal/drivers/clocksource \
           ee_dev/ee_test_board/src

include ../../ee_make/rules.mk