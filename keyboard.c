#include "keyboard.h"

struct kb_buf keyboard_buf;

void init_keyboard()
{
	keyboard_buf.count = 0;
	keyboard_buf.head = keyboard_buf.tail = keyboard_buf.buf;
}
