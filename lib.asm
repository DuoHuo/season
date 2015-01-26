	PAGE_DIR_TBL_ADDR	equ	0x100000
[BITS 32]
; void apply_paging
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

; void out_byte(u16 port, u8 value);
global out_byte
out_byte:
	mov	edx, [esp + 4]		; port
	mov	al, [esp + 4 + 4]	; value
	out	dx, al
	nop
	nop
	ret

; u8 in_byte(u16 port);
global in_byte
in_byte:
	mov	edx, [esp + 4]		; port
	xor	eax, eax
	in	al, dx
	nop
	nop
	ret

; void load_idtr(struct dtr idtr);
global load_idtr
load_idtr:
	lidt	[esp + 4]
	nop
	nop
	ret

; void load_gdtr(struct dtr gdtr);
global load_gdtr
load_gdtr:
	lgdt	[esp + 4]
	nop
	nop
	ret

; void save_gdtr();
extern gdtr
global save_gdtr
save_gdtr:
	sgdt	[gdtr]
	nop
	nop
	ret

; void load_ldtr(struct dtr ldtr);
global load_ldtr
load_ldtr:
	lldt	[esp + 4]
	nop
	nop
	ret

; void save_ldtr();
extern ldtr
global save_ldtr
save_ldtr:
	sldt	[ldtr]
	nop
	nop
	ret

; void set_interupt()
global set_interupt
set_interupt:
	sti
	ret

; void clear_interupt()
global clear_interupt
clear_interupt:
	cli
	ret
