#ifndef INTERUPT_H_H
#define INTERUPT_H_H

#define IDT_SIZE	256
#define	PRIVILEGE_KERN	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3

struct idt_entry {
	u16 offset_low;
	u16 selector;
	u8 dcount;
	u8 attr;
	u16 offset_high;
};

struct idtr {
	u16 limit;
	u16 baselow;
	u16 basehigh;
};

typedef void (*int_handler)();

struct idt_entry idt[IDT_SIZE];
struct idtr idtr_filler;

void init_idtr();
void init_8259A();
void setup_idt();
void init_idt_entry(unsigned char vector, int_handler handler,\
				unsigned char privilege);

#endif
