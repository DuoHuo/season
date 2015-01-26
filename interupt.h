#ifndef INTERUPT_H_H
#define INTERUPT_H_H

#define IDT_SIZE	256
#define	PRIVILEGE_KERN	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3

#include "protected.h"

typedef void (*int_handler)();

extern struct idt_entry idt[IDT_SIZE];
extern struct dtr idtr;

extern void init_idtr();
extern void init_8259A();
extern void setup_idt();
extern void init_idt_entry(unsigned char vector, int_handler handler,\
				unsigned char privilege);

#endif
