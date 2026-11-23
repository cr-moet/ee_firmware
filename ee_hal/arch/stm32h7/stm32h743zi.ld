MEMORY
{
	FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
	DTCM  (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(DTCM) + LENGTH(DTCM);

SECTIONS
{
	.isr_vector : { KEEP(*(.isr_vector)) } > FLASH
	.text : { *(.text*) *(.rodata*) . = ALIGN(4); } > FLASH

	_sidata = LOADADDR(.data);
	.data : { . = ALIGN(4); _sdata = .; *(.data*) . = ALIGN(4); _edata = .; } > DTCM AT > FLASH
	.bss  : { . = ALIGN(4); _sbss = .; *(.bss*) *(COMMON) . = ALIGN(4); _ebss = .; } > DTCM
}