#define KEYBOARD_BUF_LEN	50

struct kb_buf
{
	char *head;
	char *tail;
	int count;
	char buf[KEYBOARD_BUF_LEN];
};

extern struct kb_buf keyboard_buf;
extern void init_keyboard();
