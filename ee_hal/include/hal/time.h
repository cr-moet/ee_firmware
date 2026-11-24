#ifndef _HAL_TIME_H
#define _HAL_TIME_H

#include <hal/types.h>

#define HZ	1000	/* Ticks per second. One tick is 1 ms. */

/* The interrupt changes this value. For this reason, it is volatile. */
extern volatile unsigned long jiffies;

/* These macros are safe when the counter overflows. */
#define time_after(a, b)	((long)((b) - (a)) < 0)
#define time_before(a, b)	time_after(b, a)

static inline unsigned long msecs_to_jiffies(unsigned int ms)
{
	return ((unsigned long)ms * HZ + 999) / 1000;
}

void mdelay(unsigned int ms);

/* Only timer drivers use this function. */
void tick_handle_periodic(void);

#endif /* _HAL_TIME_H */