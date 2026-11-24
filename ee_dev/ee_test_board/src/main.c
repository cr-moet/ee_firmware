#include <hal/gpio.h>
#include <hal/time.h>
#include "board.h"

int main(void)
{
	unsigned long next;
	int led = 0;

	gpio_direction_output(LED_GREEN, 0);
	gpio_direction_output(LED_BLUE, 0);
	gpio_direction_input(BUTTON_USER);

	next = jiffies + msecs_to_jiffies(500);

	for (;;) {
		if (time_after(jiffies, next)) {
			led = !led;
			gpio_set_value(LED_GREEN, led);
			next += msecs_to_jiffies(500);
		}

		gpio_set_value(LED_BLUE, gpio_get_value(BUTTON_USER));
	}
}