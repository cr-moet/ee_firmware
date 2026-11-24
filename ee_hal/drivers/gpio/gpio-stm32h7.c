#include <hal/kernel.h>
#include <hal/io.h>
#include <hal/errno.h>
#include <hal/init.h>
#include <hal/gpio.h>
#include <hal/clk.h>
#include <hal/gpio/stm32h7.h>

#define STM32H7_GPIO_BASE	0x58020000UL
#define STM32H7_GPIO_STRIDE	0x400UL

#define GPIO_MODER		0x00
#define GPIO_IDR		0x10
#define GPIO_BSRR		0x18

#define MODER_INPUT		0x0
#define MODER_OUTPUT	0x1
#define MODER_MASK		0x3

struct stm32_gpio_bank {
	struct gpio_chip	gc;
	void				*base;
	const char			*clk_name;
};

#define to_bank(c)	container_of(c, struct stm32_gpio_bank, gc)

static void stm32_gpio_set_mode(struct stm32_gpio_bank *bank,
				unsigned int off, u32 mode)
{
	u32 val;

	/*
	 * Read-modify-write. Add a lock when interrupts or an RTOS
	 * use this function.
	 */
	val = readl(bank->base + GPIO_MODER);
	val &= ~(MODER_MASK << (off * 2));
	val |= mode << (off * 2);
	writel(val, bank->base + GPIO_MODER);
}

static void stm32_gpio_set(struct gpio_chip *gc, unsigned int off, int value)
{
	struct stm32_gpio_bank *bank = to_bank(gc);

	/* The BSRR register is atomic. A lock is not necessary. */
	writel(value ? BIT(off) : BIT(off + 16), bank->base + GPIO_BSRR);
}

static int stm32_gpio_get(struct gpio_chip *gc, unsigned int off)
{
	struct stm32_gpio_bank *bank = to_bank(gc);

	return !!(readl(bank->base + GPIO_IDR) & BIT(off));
}

static int stm32_gpio_direction_input(struct gpio_chip *gc, unsigned int off)
{
	stm32_gpio_set_mode(to_bank(gc), off, MODER_INPUT);
	return 0;
}

static int stm32_gpio_direction_output(struct gpio_chip *gc,
				       unsigned int off, int value)
{
	/*
	 * Set the output level first. Then set the pin to output mode.
	 * This prevents a glitch.
	 */
	stm32_gpio_set(gc, off, value);
	stm32_gpio_set_mode(to_bank(gc), off, MODER_OUTPUT);
	return 0;
}

static const struct gpio_ops stm32_gpio_ops = {
	.direction_input	= stm32_gpio_direction_input,
	.direction_output	= stm32_gpio_direction_output,
	.get			= stm32_gpio_get,
	.set			= stm32_gpio_set,
};

#define STM32_BANK(_n, _label, _clk) {				\
	.gc = {										\
		.label	= _label,						\
		.ops	= &stm32_gpio_ops,				\
		.base	= (_n) * 16,					\
		.ngpio	= 16,							\
	},											\
	.base		= (void *)(STM32H7_GPIO_BASE +	\
				   (_n) * STM32H7_GPIO_STRIDE),	\
	.clk_name	= _clk,							\
}

static struct stm32_gpio_bank banks[] = {
	STM32_BANK(0,  "GPIOA", "gpioa"), STM32_BANK(1,  "GPIOB", "gpiob"),
	STM32_BANK(2,  "GPIOC", "gpioc"), STM32_BANK(3,  "GPIOD", "gpiod"),
	STM32_BANK(4,  "GPIOE", "gpioe"), STM32_BANK(5,  "GPIOF", "gpiof"),
	STM32_BANK(6,  "GPIOG", "gpiog"), STM32_BANK(7,  "GPIOH", "gpioh"),
	STM32_BANK(8,  "GPIOI", "gpioi"), STM32_BANK(9,  "GPIOJ", "gpioj"),
	STM32_BANK(10, "GPIOK", "gpiok"),
};

int stm32h7_gpio_init(void)
{
	unsigned int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(banks); i++) {
		ret = clk_enable(clk_get(banks[i].clk_name));
		if (ret)
			return ret;

		ret = gpiochip_add(&banks[i].gc);
		if (ret)
			return ret;
	}
	return 0;
}arch_initcall(stm32h7_gpio_init);