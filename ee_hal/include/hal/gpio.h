#ifndef _HAL_GPIO_H
#define _HAL_GPIO_H

#include <hal/types.h>

struct gpio_chip;

struct gpio_ops {
	int  (*direction_input)(struct gpio_chip *gc, unsigned int offset);
	int  (*direction_output)(struct gpio_chip *gc, unsigned int offset, int value);
	int  (*get)(struct gpio_chip *gc, unsigned int offset);
	void (*set)(struct gpio_chip *gc, unsigned int offset, int value);
};

struct gpio_chip {
	const char		        *label;
	const struct gpio_ops	*ops;
	unsigned int		    base;	/* The first global GPIO number of this chip. */
	unsigned int		    ngpio;
	struct gpio_chip	    *next;
};

/* Provider API. Drivers use these functions. */
int gpiochip_add(struct gpio_chip *gc);

/* Consumer API. Boards and applications use these functions. */
int  gpio_direction_input(unsigned int gpio);
int  gpio_direction_output(unsigned int gpio, int value);
int  gpio_get_value(unsigned int gpio);
void gpio_set_value(unsigned int gpio, int value);

#endif /* _HAL_GPIO_H */