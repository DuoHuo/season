	PAGE_DIR_TBL_ADDR	equ	0x100000
[BITS 32]
global apply_paging
apply_paging:
	push	eax
	mov	eax, PAGE_DIR_TBL_ADDR
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 0x80000000
	mov	cr0, eax
	pop	eax
	ret

