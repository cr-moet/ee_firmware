#include <hal/init.h>

/* The linker script defines these symbols. */
extern const initcall_t __initcall_start[];
extern const initcall_t __initcall_end[];

int do_initcalls(void)
{
	const initcall_t *fn;
	int err = 0;
	int ret;

	/* Call all functions. Keep the first error. */
	for (fn = __initcall_start; fn < __initcall_end; fn++) {
		ret = (*fn)();
		if (ret && !err)
			err = ret;
	}
	return err;
}