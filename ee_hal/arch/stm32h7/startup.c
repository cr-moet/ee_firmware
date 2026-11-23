/* ee_hal/arch/stm32h7/startup.c */

/* 1. Symbole aus dem Linkerskript */
extern unsigned long _estack;
extern unsigned long _sidata, _sdata, _edata;
extern unsigned long _sbss, _ebss;

int main(void);

/* 2. Handler */
void Reset_Handler(void)
{
	unsigned long *src = &_sidata;
	unsigned long *dst = &_sdata;

	/* .data vom Flash ins RAM kopieren */
	while (dst < &_edata)
		*dst++ = *src++;

	/* .bss mit Nullen füllen */
	for (dst = &_sbss; dst < &_ebss; )
		*dst++ = 0;

	/* FPU einschalten (CP10/CP11), wegen -mfloat-abi=hard */
	*(volatile unsigned long *)0xE000ED88 |= 0xFUL << 20;

	main();

	for (;;)
		;
}

static void default_handler(void)
{
	for (;;)
		;
}

/* 3. Vektortabelle, ganz am Ende */
__attribute__((section(".isr_vector"), used))
static void (* const vectors[16])(void) = {
	(void (*)(void))&_estack,
	Reset_Handler,
	default_handler,	/* NMI */
	default_handler,	/* HardFault */
	default_handler,	/* MemManage */
	default_handler,	/* BusFault */
	default_handler,	/* UsageFault */
	0, 0, 0, 0,		/* reserviert */
	default_handler,	/* SVCall */
	default_handler,	/* DebugMon */
	0,			/* reserviert */
	default_handler,	/* PendSV */
	default_handler,	/* SysTick */
};