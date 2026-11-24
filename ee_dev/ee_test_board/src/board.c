#include "board.h"

int board_init(void)
{
	return stm32h7_gpio_init();
}