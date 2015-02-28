#include "lib.h"
#include "types.h"
#include "sys_call.h"

#define TIMER0_PORT	0x40
#define TIMER_CTL_PORT	0x43
#define TIMER_FREQ	1193180L
#define HZ		100

void init_pit()
{
	out_byte(TIMER_CTL_PORT, 0x34); /* 00110100 means counter0, write lower byte first,
				    	 * using 2 mode, using binary */
	out_byte(TIMER0_PORT, (u8)(TIMER_FREQ / HZ)); /* write lower byte*/
	out_byte(TIMER0_PORT, (u8)((TIMER_FREQ / HZ) >> 8));
}

/*TODO: This delay will block interupt */
void delay(int milli_sec)
{
	int t = get_ticks();

	while (((get_ticks() - t) * 1000 / HZ) < milli_sec) { }
}
