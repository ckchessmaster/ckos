# Declare constants for the multiboot header.
.set MAGIC,			0xe85250d6               	# 'magic number' lets bootloader find the header (Note: This is the multiboot2 number v1 is 0x1BADB002)
.set ARCH,			0						 	# architecture, 0=x86, 4=M
.set HEADER_LENGTH, header_end - header_start	# header length in bytes
.set CHECKSUM, 		0x100000000 - (MAGIC + ARCH + HEADER_LENGTH)  			# checksum

.section .multiboot_header
.align 8
header_start:
	.long MAGIC
	.long ARCH
	.long HEADER_LENGTH
	.long CHECKSUM

	# Optional multiboot tags
	
	.align 8
	framebuffer_tag_start:
		.short 5 	# type - framebuffer
		.short 1 	# flags
		.long framebuffer_tag_end - framebuffer_tag_start 	# size
		.long 1280 	# width - 0 lets the bootloader decide
		.long 720	# height - 0 lets the bootloader decide
		.long 32	# depth - 0 lets the bootloader decide
	framebuffer_tag_end:
	
	# End tags
	.align 8
	.short 0 # type 
	.short 0 # flags
	.long 8  # size
header_end:

/* Setup the stack */
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/* Kernel entrypoint */
.section .text
.global _start
.type _start, @function
_start:
	/* Initialize the stack */
	mov $stack_top, %esp
	pushl %ebx # Push the pointer to the multiboot info structure
	pushl %eax # Push the magic value

	/* Start the kernel */
	call kernel_main

	/* Infinite loop */
	cli 	/* Disable interrupts */
1:	hlt 	/* Wait for the next interrupt to arrive */
	jmp 1b 	/* Loop to hlt */

/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start
