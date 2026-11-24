#include <hal/kernel.h>
#include <hal/io.h>
#include <hal/errno.h>
#include <hal/time.h>
#include <hal/clockevent.h>
#include <hal/init.h>
#include <hal/clk.h>

#define SYST_BASE		((void *)0xE000E010UL)
#define SYST_CSR		0x00	/* Control and status */
#define SYST_RVR		0x04	/* Reload value */
#define SYST_CVR		0x08	/* Current value */

#define SYST_CSR_ENABLE		BIT(0)
#define SYST_CSR_TICKINT	BIT(1)
#define SYST_CSR_CLKSOURCE	BIT(2)	/* 1 = processor clock */

#define SYST_RVR_MAX		0x00FFFFFFUL	/* 24 bits */

static int systick_set_periodic(struct clock_event_device *ced, unsigned int hz)
{
	unsigned long reload;

	if (!hz)
		return -EINVAL;

	reload = ced->rate / hz - 1;
	if (reload > SYST_RVR_MAX)
		return -EINVAL;

	writel(0, SYST_BASE + SYST_CSR);		/* Stop the timer first. */
	writel(reload, SYST_BASE + SYST_RVR);
	writel(0, SYST_BASE + SYST_CVR);		/* Set the counter to zero. */
	writel(SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE,
	       SYST_BASE + SYST_CSR);
	return 0;
}

static const struct clock_event_ops systick_ops = {
	.set_periodic	= systick_set_periodic,
};

static struct clock_event_device systick = {
	.name	= "systick",
	.ops	= &systick_ops,
};

/* This function replaces the weak alias in startup.c. */
void SysTick_Handler(void)
{
	tick_handle_periodic();
}

static int systick_init(void)
{
	struct clk *clk = clk_get("cpu");

	if (!clk)
		return -ENODEV;

	systick.rate = clk_get_rate(clk);
	return clockevent_register(&systick);
}postcore_initcall(systick_init);