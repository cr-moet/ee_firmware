ARCH    := stm32h7
TARGET  := ee_test_board

# STM32CubeProgrammer: SWD, Verbindung unter Reset
CUBEPROG_CONNECT  := port=SWD mode=UR reset=HWrst

# OpenOCD
OPENOCD_INTERFACE := interface/stlink.cfg
OPENOCD_EXTRA     := -c "transport select swd"
OPENOCD_RESET     := -c "reset_config srst_only srst_nogate connect_assert_srst"

subdirs := ee_hal/arch/$(ARCH) \
           ee_dev/ee_test_board/src

include ../../ee_make/rules.mk