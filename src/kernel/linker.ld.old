ENTRY(_start)

SECTIONS
{
	/* Start offset */
	. = 2M;

	/* Multiboot2-Header must be 64-bit (8 byte) aligned according to spec. */
    . = ALIGN(8);

	.text . :
	{
		*(.multiboot_header)
		*(.text .text.*)
	}

	/* Read-only data. */
	.rodata :
	{
		*(.rodata .rodata.*)
	}

	/* Read-write data (initialized) */
	.data :
	{
		*(.data .data.*)
	}

	/* Read-write data (uninitialized) and stack */
	.bss :
	{
		*(COMMON)
		*(.bss .bss.*)
	}

	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}