#ifndef _HAL_CLOCKEVENT_H
#define _HAL_CLOCKEVENT_H

#include <hal/types.h>

struct clock_event_device;

struct clock_event_ops {
	int (*set_periodic)(struct clock_event_device *ced, unsigned int hz);
};

struct clock_event_device {
	const char			            *name;
	const struct clock_event_ops	*ops;
	unsigned long			        rate;	/* Input clock in Hz */
};

int clockevent_register(struct clock_event_device *ced);

#endif /* _HAL_CLOCKEVENT_H */