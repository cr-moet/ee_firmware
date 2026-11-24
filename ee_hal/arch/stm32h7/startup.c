/* ee_hal/arch/stm32h7/startup.c */

/* 1. Symbols from the linker script. */
extern unsigned long _estack;
extern unsigned long _sidata, _sdata, _edata;
extern unsigned long _sbss, _ebss;

int main(void);

/* 2. Exception handlers. */
void Reset_Handler(void)
{
	unsigned long *src = &_sidata;
	unsigned long *dst = &_sdata;

	/* Copy the .data section from the flash memory to the RAM. */
	while (dst < &_edata)
		*dst++ = *src++;

	/* Set the .bss section to zero. */
	for (dst = &_sbss; dst < &_ebss; )
		*dst++ = 0;

	/* Enable the FPU (CP10 and CP11). The build uses -mfloat-abi=hard. */
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

#define WEAK_ALIAS	__attribute__((weak, alias("default_handler")))

void NMI_Handler(void)		WEAK_ALIAS;
void HardFault_Handler(void)	WEAK_ALIAS;
void MemManage_Handler(void)	WEAK_ALIAS;
void BusFault_Handler(void)	WEAK_ALIAS;
void UsageFault_Handler(void)	WEAK_ALIAS;
void SVC_Handler(void)		WEAK_ALIAS;
void DebugMon_Handler(void)	WEAK_ALIAS;
void PendSV_Handler(void)	WEAK_ALIAS;
void SysTick_Handler(void)	WEAK_ALIAS;

/* 3. Vector table. Declare all handlers before this table. */
__attribute__((section(".isr_vector"), used))
static void (* const vectors[16])(void) = {
	(void (*)(void))&_estack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0, 0, 0, 0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
};