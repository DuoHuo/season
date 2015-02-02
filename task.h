#ifndef TASK_H_H
#define TASK_H_H

#include "types.h"
#include "protected.h"

#define TASK_STACK_SIZE 200
#define TASK_NUM	2
#define KERNEL_STACK_SIZE	200
#define TASK_NAME_SIZE	16
#define LDT_SIZE	4

struct tss {
	u32 backlink;
	u32 esp0;
	u32 ss0;
	u32 esp1;
	u32 ss1;
	u32 esp2;
	u32 ss2;
	u32 cr3;
	u32 eip;
	u32 flags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u32 es;
	u32 cs;
	u32 ss;
	u32 ds;
	u32 fs;
	u32 gs;
	u32 ldt;
	u16 trap;
	u16 iobase;
};

struct regs {
	u32 gs;
	u32 fs;
	u32 es;
	u32 ds;
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 kernel_esp;
	u32 ebx;
	u32 edx;
	u32 ecx;
	u32 eax;
	u32 retaddr;
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 esp;
	u32 ss;
};

struct tcb {
	struct regs regs;
	selector_t ldt_sel;
	struct descriptor ldt[LDT_SIZE];
	u32 pid;
	char name[TASK_NAME_SIZE];
	u16 stack[TASK_STACK_SIZE];
};

typedef void (*task_fun_t)();

extern struct tcb task_tbl[TASK_NUM];
extern struct tcb *ready_task;
extern char kernel_stack[KERNEL_STACK_SIZE];
extern struct tss itss;

extern void init_task(struct tcb *ptcb, task_fun_t task_fun, int pid, char *name);
extern void init_tasks();
extern void scheduler();

#endif
