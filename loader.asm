%include "boot.inc"
%include "pm.inc"

	org	07c00h
	mov	ax, cs
	mov	ds, ax

	xor	ax, ax
	xor	dl, dl
	int	13

	mov	ax, kernel_base
	mov	es, ax
	mov	ah, 02h
	mov	al, 01h
	mov	ch, 00h
	mov	cl, 02h
	mov	dh, 00h
	mov	dl, 00h
	mov	bx, 0
	int	13h

	jmp	SWITCH_PM


LABEL_GDT:		boot_descriptor	0,	0,	0
LABEL_DESC_FLAT_C:	boot_descriptor	0,	0fffffh,DA_32|DA_CR|DA_LIMIT_4K
LABEL_DESC_FLAT_RW:	boot_descriptor	0,	0fffffh,DA_32|DA_DRW|DA_LIMIT_4K
LABEL_DESC_VIDEO:	boot_descriptor	0B8000h,0ffffh,	DA_DRW|DA_DPL3

gdt_len		equ	$ - LABEL_GDT
gdt_ptr		dw	gdt_len - 1
		dd	LABEL_GDT

selector_flat_c		equ	LABEL_DESC_FLAT_C - LABEL_GDT
selector_flat_rw	equ	LABEL_DESC_FLAT_RW - LABEL_GDT
selector_video		equ	LABEL_DESC_VIDEO - LABEL_GDT + SA_RPL3


SWITCH_PM:
	lgdt	[gdt_ptr]
	cli
	in	al, 92h
	or	al, 02h
	out	92h, al
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax
	
	jmp	dword selector_flat_c:PM_START

[BITS	32]
PM_START:
	mov	ax, selector_video
	mov	gs, ax
	mov	ax, selector_flat_rw
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	ss, ax
	mov	esp, 0x7c00

	mov	ah, 0fh
	mov	al, 'P'
	mov	[gs:((80*0+39)*2)], ax


	jmp	kernel_base_phy

times	510-($-$$)	db	0
		dw	0xaa55
