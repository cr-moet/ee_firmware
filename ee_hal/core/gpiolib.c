#include <hal/errno.h>
#include <hal/types.h>
#include <hal/errno.h>
#include <hal/gpio.h>

static struct gpio_chip *gpio_chips;

static struct gpio_chip *gpio_to_chip(unsigned int gpio, unsigned int *offset)
{
	struct gpio_chip *gc;

	for (gc = gpio_chips; gc; gc = gc->next) {
		if (gpio >= gc->base && gpio < gc->base + gc->ngpio) {
			*offset = gpio - gc->base;
			return gc;
		}
	}
	return NULL;
}

int gpiochip_add(struct gpio_chip *gc)
{
	struct gpio_chip *it;

	if (!gc || !gc->ops || !gc->ngpio)
		return -EINVAL;

	/* Do not accept a chip if its number range overlaps a registered chip. */
	for (it = gpio_chips; it; it = it->next) {
		if (gc->base < it->base + it->ngpio &&
		    it->base < gc->base + gc->ngpio)
			return -EBUSY;
	}

	gc->next = gpio_chips;
	gpio_chips = gc;
	return 0;
}

int gpio_direction_input(unsigned int gpio)
{
	unsigned int off;
	struct gpio_chip *gc = gpio_to_chip(gpio, &off);

	if (!gc)
		return -ENODEV;
	if (!gc->ops->direction_input)
		return -EOPNOTSUPP;

	return gc->ops->direction_input(gc, off);
}

int gpio_direction_output(unsigned int gpio, int value)
{
	unsigned int off;
	struct gpio_chip *gc = gpio_to_chip(gpio, &off);

	if (!gc)
		return -ENODEV;
	if (!gc->ops->direction_output)
		return -EOPNOTSUPP;

	return gc->ops->direction_output(gc, off, value);
}

int gpio_get_value(unsigned int gpio)
{
	unsigned int off;
	struct gpio_chip *gc = gpio_to_chip(gpio, &off);

	if (!gc)
		return -ENODEV;
	if (!gc->ops->get)
		return -EOPNOTSUPP;

	return gc->ops->get(gc, off);
}

void gpio_set_value(unsigned int gpio, int value)
{
	unsigned int off;
	struct gpio_chip *gc = gpio_to_chip(gpio, &off);

	if (gc && gc->ops->set)
		gc->ops->set(gc, off, value);
}