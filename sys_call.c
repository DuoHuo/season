#include "sys_call.h"
#include "kernel.h"
#include "interupt.h"

/* These value should be in sync with those in sys_call_entry.asm */
#define SYS_GET_TICKS	0

sys_call_handler_t sys_call_tbl[SYS_CALL_NUM];

static int sys_get_ticks()
{
	init_idtr();
	return ticks;
}

void init_sys_call_tbl()
{
	sys_call_tbl[SYS_GET_TICKS] = sys_get_ticks;
}
