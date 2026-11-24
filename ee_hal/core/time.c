#include <hal/types.h>
#include <hal/errno.h>
#include <hal/time.h>
#include <hal/clockevent.h>

volatile unsigned long jiffies;

static struct clock_event_device *tick_dev;

void tick_handle_periodic(void)
{
	jiffies++;
}

int clockevent_register(struct clock_event_device *ced)
{
	int ret;

	if (!ced || !ced->ops || !ced->ops->set_periodic || !ced->rate)
		return -EINVAL;
	if (tick_dev)
		return -EBUSY;

	ret = ced->ops->set_periodic(ced, HZ);
	if (ret)
		return ret;

	tick_dev = ced;
	return 0;
}

void mdelay(unsigned int ms)
{
	unsigned long timeout;

	if (!tick_dev)		/* Without a timer, this loop does not stop. */
		return;

	/* Add 1 tick. The current tick has already started. */
	timeout = jiffies + msecs_to_jiffies(ms) + 1;
	while (time_before(jiffies, timeout))
		;
}