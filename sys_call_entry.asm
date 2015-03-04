; these value should be in sync with those in sys_call.c
SYS_GET_TICKS	equ	0
SYS_WRITE	equ	1

; int get_ticks()
global get_ticks
get_ticks:
	mov	eax, SYS_GET_TICKS
	int	0x80
	ret

; void write(char *buf, int len)
global write
write:
	mov	eax, SYS_WRITE
	mov	ebx, [esp + 4] 
	mov	ecx, [esp + 8] 
	int	0x80
	ret
