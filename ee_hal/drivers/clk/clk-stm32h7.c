#include <hal/kernel.h>
#include <hal/io.h>
#include <hal/errno.h>
#include <hal/init.h>
#include <hal/clk.h>

#define RCC_BASE		((void *)0x58024400UL)
#define RCC_AHB4ENR		0x0E0
#define RCC_APB1LENR		0x0E8

#define STM32H7_HSI_HZ		64000000UL

/* Root clock: HSI with a fixed rate. */
static unsigned long stm32_hsi_get_rate(struct clk *clk)
{
	(void)clk;
	/* After reset, the HSI divider is 1. */
	return STM32H7_HSI_HZ;
}

static const struct clk_ops stm32_hsi_ops = {
	.get_rate	= stm32_hsi_get_rate,
};

static struct clk hsi  = { .name = "hsi",  .ops = &stm32_hsi_ops };

/*
 * After reset, the CPU, AHB and APB clocks use the HSI clock without a
 * divider. For this reason, these clocks have no ops. They use the rate
 * of the parent. Add ops when you add PLL support.
 */
static struct clk cpu   = { .name = "cpu",   .parent = &hsi };
static struct clk hclk  = { .name = "hclk",  .parent = &cpu };
static struct clk pclk1 = { .name = "pclk1", .parent = &hclk };

/* Gate clocks: one enable bit in an RCC register. */
struct stm32_clk_gate {
	struct clk	clk;
	u32		reg;
	unsigned int	bit;
};

#define to_gate(c)	container_of(c, struct stm32_clk_gate, clk)

static int stm32_gate_enable(struct clk *clk)
{
	struct stm32_clk_gate *gate = to_gate(clk);
	u32 val;

	val = readl(RCC_BASE + gate->reg);
	writel(val | BIT(gate->bit), RCC_BASE + gate->reg);

	/* Read the register again. This makes sure that the clock is on. */
	(void)readl(RCC_BASE + gate->reg);
	return 0;
}

static void stm32_gate_disable(struct clk *clk)
{
	struct stm32_clk_gate *gate = to_gate(clk);
	u32 val;

	val = readl(RCC_BASE + gate->reg);
	writel(val & ~BIT(gate->bit), RCC_BASE + gate->reg);
}

static const struct clk_ops stm32_gate_ops = {
	.enable		= stm32_gate_enable,
	.disable	= stm32_gate_disable,
};

#define STM32_GATE(_name, _parent, _reg, _bit) {	\
	.clk = {							            \
		.name	= _name,					        \
		.ops	= &stm32_gate_ops,				    \
		.parent	= _parent,					        \
	},								                \
	.reg	= _reg,							        \
	.bit	= _bit,							        \
}

static struct stm32_clk_gate gates[] = {
	STM32_GATE("gpioa", &hclk, RCC_AHB4ENR, 0),
	STM32_GATE("gpiob", &hclk, RCC_AHB4ENR, 1),
	STM32_GATE("gpioc", &hclk, RCC_AHB4ENR, 2),
	STM32_GATE("gpiod", &hclk, RCC_AHB4ENR, 3),
	STM32_GATE("gpioe", &hclk, RCC_AHB4ENR, 4),
	STM32_GATE("gpiof", &hclk, RCC_AHB4ENR, 5),
	STM32_GATE("gpiog", &hclk, RCC_AHB4ENR, 6),
	STM32_GATE("gpioh", &hclk, RCC_AHB4ENR, 7),
	STM32_GATE("gpioi", &hclk, RCC_AHB4ENR, 8),
	STM32_GATE("gpioj", &hclk, RCC_AHB4ENR, 9),
	STM32_GATE("gpiok", &hclk, RCC_AHB4ENR, 10),
	STM32_GATE("usart3", &pclk1, RCC_APB1LENR, 18),
};

static int stm32h7_clk_init(void)
{
	static struct clk * const roots[] = { &hsi, &cpu, &hclk, &pclk1 };
	unsigned int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(roots); i++) {
		ret = clk_register(roots[i]);
		if (ret)
			return ret;
	}

	for (i = 0; i < ARRAY_SIZE(gates); i++) {
		ret = clk_register(&gates[i].clk);
		if (ret)
			return ret;
	}
	return 0;
}
core_initcall(stm32h7_clk_init);