extern	k_reenter
extern	exception_handler
extern	kernel_stack
extern	itss
extern	ready_task
extern	isr_tbl
extern	sys_call_tbl
extern	scheduler
KERNEL_STACK_SIZE	equ	200

global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error
global  hwint00
global  hwint01
global  hwint02
global  hwint03
global  hwint04
global  hwint05
global  hwint06
global  hwint07
global  hwint08
global  hwint09
global  hwint10
global  hwint11
global  hwint12
global  hwint13
global  hwint14
global  hwint15
global	sys_call

%macro  hwint_master    1
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<%1)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	%1
	call	(isr_tbl + 4 * %1)
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<%1)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd
%endmacro
; ---------------------------------

ALIGN   16
hwint00:                ; Interrupt routine for irq 0 (the clock).
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<0)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter0

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	0
	call	[isr_tbl + 4 * 0]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter0:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<0)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd
;hwint00:
;	sub	esp, 4
;	pushad
;	push	ds
;	push	es
;	push	fs
;	push	gs
;	mov	dx, ss		; ss's DPL is 0 which others want to be
;	mov	ds, dx
;	mov	es, dx
;
;	in	al, 0x21	; block same interupt source
;	or	al, 1
;	out	0x21, al
;
;	mov	al, 0x20	; EndOfInterupt
;	out	0x20, al
;
;	inc	dword [k_reenter]
;	cmp	dword [k_reenter], 0
;	jne	.re_enter
;
;; move to kernel stack
;	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)
;
;	sti
;; call functions
;;	call	scheduler
;	call	[isr_tbl]
;
;	cli
;; leave kernel stack to pcb.regs
;	mov	esp, [ready_task]
;	lldt	[esp + (18*4)]
;	lea	eax, [esp + (18*4)]
;	mov	dword [itss + 4], eax
;
;.re_enter:
;
;	in	al, 0x21	; restore the same interupt
;	and	al, 0xfe
;	out	0x21, al
;
;	dec	dword [k_reenter]
;	pop	gs
;	pop	fs
;	pop	es
;	pop	ds
;	popad
;	add	esp, 4
;	iretd

ALIGN   16
hwint01:                ; Interrupt routine for irq 1 (keyboard)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<1)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter1

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	1
	call	[isr_tbl + 4 * 1]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter1:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<1)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint02:                ; Interrupt routine for irq 2 (cascade!)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<2)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter2

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	2
	call	[isr_tbl + 4 * 2]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter2:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<2)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint03:                ; Interrupt routine for irq 3 (second serial)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<3)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter3

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	3
	call	[isr_tbl + 4 * 3]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter3:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<3)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint04:                ; Interrupt routine for irq 4 (first serial)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<4)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter4

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	4
	call	[isr_tbl + 4 * 4]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter4:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<4)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint05:                ; Interrupt routine for irq 5 (XT winchester)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<5)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter5

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	5
	call	[isr_tbl + 4 * 5]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter5:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<5)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint06:                ; Interrupt routine for irq 6 (floppy)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<6)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter6

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	6
	call	[isr_tbl + 4 * 6]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter6:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<6)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint07:                ; Interrupt routine for irq 7 (printer)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0x21	; block same interupt source
	or	al, (1<<7)
	out	0x21, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter7

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	7
	call	[isr_tbl + 4 * 7]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter7:
	in	al, 0x21	; restore the same interupt
	and	al, ~(1<<7)
	out	0x21, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

; ---------------------------------
%macro  hwint_slave     1
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(%1-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter%1

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	%1
	call	(isr_tbl + 4 * %1)
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter%1:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(%1-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd
%endmacro
; ---------------------------------

ALIGN   16
hwint08:                ; Interrupt routine for irq 8 (realtime clock).
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(8-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter8

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	8
	call	[isr_tbl + 4 * 8]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter8:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(8-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint09:                ; Interrupt routine for irq 9 (irq 2 redirected)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(9-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter9

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	9
	call	[isr_tbl + 4 * 9]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter9:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(9-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint10:                ; Interrupt routine for irq 10
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(10-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter10

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	10
	call	[isr_tbl + 4 * 10]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter10:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(10-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint11:                ; Interrupt routine for irq 11
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(11-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter11

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	11
	call	[isr_tbl + 4 * 11]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter11:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(11-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint12:                ; Interrupt routine for irq 12
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(12-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter12

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	12
	call	[isr_tbl + 4 * 12]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter12:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(12-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint13:                ; Interrupt routine for irq 13 (FPU exception)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(13-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter13

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	13
	call	[isr_tbl + 4 * 13]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter13:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(13-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint14:                ; Interrupt routine for irq 14 (AT winchester)
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(14-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter14

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	14
	call	[isr_tbl + 4 * 14]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter14:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(14-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd

ALIGN   16
hwint15:                ; Interrupt routine for irq 15
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	in	al, 0xa1	; block same interupt source
	or	al, (1<<(15-8))
	out	0xa1, al

	mov	al, 0x20	; EndOfInterupt
	out	0x20, al
	nop			; delay
	nop
	out	0xa0, al

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter15

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	15
	call	[isr_tbl + 4 * 15]
	add	esp, 4
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter15:
	in	al, 0xa1	; restore the same interupt
	and	al, ~(1<<(15-8))
	out	0xa1, al

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd



; 中断和异常 -- 异常
divide_error:
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	jmp	exception
single_step_exception:
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	jmp	exception
nmi:
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	jmp	exception
breakpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	jmp	exception
overflow:
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	jmp	exception
bounds_check:
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	jmp	exception
inval_opcode:
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	jmp	exception
copr_not_available:
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	jmp	exception
double_fault:
	push	8		; vector_no	= 8
	jmp	exception
copr_seg_overrun:
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	jmp	exception
inval_tss:
	push	10		; vector_no	= A
	jmp	exception
segment_not_present:
	push	11		; vector_no	= B
	jmp	exception
stack_exception:
	push	12		; vector_no	= C
	jmp	exception
general_protection:
	push	13		; vector_no	= D
	jmp	exception
page_fault:
	push	14		; vector_no	= E
	jmp	exception
copr_error:
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	jmp	exception

exception:
	call	exception_handler
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	hlt

ALIGN	16
sys_call:
	sub	esp, 4
	pushad
	push	ds
	push	es
	push	fs
	push	gs
	mov	dx, ss		; ss's DPL is 0 which others want to be
	mov	ds, dx
	mov	es, dx

	inc	dword [k_reenter]
	cmp	dword [k_reenter], 0
	jne	.re_enter_sys_call

	mov	esp, (kernel_stack + KERNEL_STACK_SIZE)

	sti
	push	ecx
	push	ebx
	call	[sys_call_tbl + 4 * eax]
	add	esp, 4 * 2
	cli

	mov	esp, [ready_task]
	lldt	[esp + (18*4)]
	mov	[esp + (11*4)], eax
	lea	eax, [esp + (18*4)]
	mov	dword [itss + 4], eax
.re_enter_sys_call:

	dec	dword [k_reenter]
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd
