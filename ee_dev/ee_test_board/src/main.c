#include <hal/gpio.h>
#include "board.h"

static void delay(volatile unsigned long n)
{
	while (n--)
		;
}

int main(void)
{
	if (board_init())
		for (;;)
			;

	gpio_direction_output(LED_GREEN, 0);

	for (;;) {
		gpio_set_value(LED_GREEN, 1);
		delay(2000000);
		gpio_set_value(LED_GREEN, 0);
		delay(2000000);
	}
}