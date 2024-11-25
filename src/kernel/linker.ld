ENTRY(_start)

SECTIONS
{
	/* Start offset */
	. = 2M;
	_startKernel = .;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot_header)
		*(.text)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		_endKernel = .;
	}
}