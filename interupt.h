#ifndef INTERUPT_H_H
#define INTERUPT_H_H

#define	PRIVILEGE_KERN	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3

#define ISR_NUM	100

#include "types.h"

typedef void (*int_handler)();

extern int_handler isr_tbl[ISR_NUM];
extern int k_reenter;

extern void init_idtr();
extern void init_8259A();
extern void setup_idt();
extern void switch_ldt_in_gdt();
extern void add_isr();
void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags);
#endif
