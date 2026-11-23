#include <stdint.h>

#define RCC_AHB4ENR	(*(volatile uint32_t *)(0x58024400UL + 0x0E0))
#define GPIOB_MODER	(*(volatile uint32_t *)(0x58020400UL + 0x00))
#define GPIOB_BSRR	(*(volatile uint32_t *)(0x58020400UL + 0x18))

static void delay(volatile uint32_t n)
{
	while (n--)
		;
}

int main(void)
{
	RCC_AHB4ENR |= 1UL << 1;		/* GPIOBEN */
	(void)RCC_AHB4ENR;			/* zurücklesen, damit der Takt sicher aktiv ist */

	GPIOB_MODER &= ~(3UL << (0 * 2));
	GPIOB_MODER |=  (1UL << (0 * 2));	/* PB0 als Ausgang */

	for (;;) {
		GPIOB_BSRR = 1UL << 0;		/* LED an */
		delay(2000000);
		GPIOB_BSRR = 1UL << 16;		/* LED aus */
		delay(2000000);
	}
}