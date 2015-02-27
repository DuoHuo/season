; these value should be in sync with those in sys_call.c
SYS_GET_TICKS	equ	0

; void get_ticks()
global get_ticks
get_ticks:
	mov	eax, SYS_GET_TICKS
	int	0x80
	ret
