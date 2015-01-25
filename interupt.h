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

extern struct idt_entry idt[IDT_SIZE];
extern struct idtr idtr_filler;

extern void init_idtr();
extern void init_8259A();
extern void setup_idt();
extern void init_idt_entry(unsigned char vector, int_handler handler,\
				unsigned char privilege);

#endif
