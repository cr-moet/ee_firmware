#ifndef _HAL_INIT_H
#define _HAL_INIT_H

typedef int (*initcall_t)(void);

/*
 * Put a pointer to fn into the section .initcall<level>.init.
 * The linker script sorts these sections by level.
 */
#define __define_initcall(fn, level)					\
	static const initcall_t __initcall_##fn##level			\
	__attribute__((used, section(".initcall" #level ".init"))) = fn

#define core_initcall(fn)	__define_initcall(fn, 1)	/* Clocks */
#define postcore_initcall(fn)	__define_initcall(fn, 2)	/* Timers */
#define arch_initcall(fn)	__define_initcall(fn, 3)	/* GPIO, pinctrl */
#define subsys_initcall(fn)	__define_initcall(fn, 4)	/* Buses */
#define device_initcall(fn)	__define_initcall(fn, 6)	/* Device drivers */
#define late_initcall(fn)	__define_initcall(fn, 7)	/* Board setup */

int do_initcalls(void);

#endif /* _HAL_INIT_H */