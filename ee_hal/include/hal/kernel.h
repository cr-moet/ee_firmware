#ifndef _HAL_KERNEL_H
#define _HAL_KERNEL_H

#include <hal/types.h>

#define BIT(n)		(1UL << (n))
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof((a)[0]))

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member)))

#endif /* _HAL_KERNEL_H */