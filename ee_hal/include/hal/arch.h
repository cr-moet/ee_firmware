#ifndef _HAL_ARCH_H
#define _HAL_ARCH_H

/* Each architecture supplies this function. A clock driver will replace it. */
unsigned long arch_cpu_rate(void);

#endif /* _HAL_ARCH_H */