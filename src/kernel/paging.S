.align 4096
.global page_directory
page_directory:
	.skip 4096

.align 4096
.global page_table_1
page_table_1:
	.skip 4096

.text
.global loadPageDirectory
loadPageDirectory:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

.text
.global enablePaging
enablePaging:
	push %ebp
	mov %eax, page_directory
	mov %cr3, %eax
	mov %eax, %cr0
	or %eax, 0x80000001
	mov %cr0, %eax
	pop %ebp
	ret
