#include <hal/clockevent/systick.h>
#include "board.h"

#define BOARD_CPU_HZ	64000000UL	/* HSI clock after reset */

int board_init(void)
{
	int ret;

	ret = stm32h7_gpio_init();
	if (ret)
		return ret;

	return systick_init(BOARD_CPU_HZ);
}