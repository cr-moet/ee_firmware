#include <hal/arch.h>

#define STM32H7_HSI_HZ	64000000UL

unsigned long arch_cpu_rate(void)
{
	/* After reset, the CPU uses the HSI clock. */
	return STM32H7_HSI_HZ;
}