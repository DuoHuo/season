#include "task.h"
#include "kernel.h"
#include "sys_call_entry.h"
#include "time.h"
#include "lib.h"
#include "libc.h"

char kernel_stack[KERNEL_STACK_SIZE];
struct tcb task_tbl[TASK_NUM];
struct tss itss;
struct tcb *ready_task;

static void idle_task_fun()
{
	int i;

	for (i = 0; ; i ++) {

	}
}

static void second_task_fun()
{
	int i;

	for (i = 0; ; i++) {

	}
}

/*
 * Initiate tcb structure for a specific task
**/
void init_task(struct tcb *ptcb, task_fun_t task_fun, int pid, char *name)
{
	selector_t cs_sel;
	selector_t ds_sel;
	selector_t es_sel;
	selector_t fs_sel;
	selector_t ss_sel;
	selector_t gs_sel;


	ptcb->pid = pid;
	//ptcb->name = name;
	/* initiate ldt in tcb */
	init_descriptor(&(ptcb->ldt[0]), 0, 0x0fffff, DA_32 | DA_LIMIT_4K | DA_C | DA_DPL1);
	init_descriptor(&(ptcb->ldt[1]), 0, 0x0fffff, DA_32 | DA_LIMIT_4K | DA_DRW| DA_DPL1);
	ptcb->ldt_sel = 32;	/* share the same selector to LDT in GDT, */
					/* for we will modified the LDT in GDT for*/
					/* every task in switch */

	/* initiate registers */
	cs_sel = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	ds_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	es_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	fs_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	ss_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	gs_sel = (24 & SA_RPL_MASK) | SA_RPL0;
	ptcb->regs.cs = cs_sel;
	ptcb->regs.ds = ds_sel;
	ptcb->regs.es = es_sel;
	ptcb->regs.fs = fs_sel;
	ptcb->regs.ss = ss_sel;
	ptcb->regs.gs = gs_sel;
	ptcb->regs.eip = (u32) task_fun;
	ptcb->regs.esp = (u32) ptcb->stack + TASK_STACK_SIZE;
	ptcb->regs.eflags = 0x1202; // IF=0, IOPL=1, bit 2 is always 1
}

void init_tasks()
{
	/* init the task_tbl */
	init_task(&task_tbl[0], idle_task_fun, 1, 0);
	init_task(&task_tbl[1], second_task_fun, 2, 0);
	ready_task = &task_tbl[0];
	/* introduce ldt gdt */
	init_descriptor(&gdt[4], (u32)(ready_task->ldt), sizeof(struct descriptor) * LDT_SIZE - 1, DA_LDT);
	/* init tss */
	itss.ss0 = 16; // Selector for Data
	init_descriptor(&gdt[5], (u32)&itss, sizeof(itss)-1, DA_386TSS);
	itss.iobase = sizeof(itss); // No I/O map
	load_tss();
}

void scheduler()
{
	int i;

	i = (ready_task->pid - 1);
	i = (i + 1) % TASK_NUM;
	ready_task = &task_tbl[i];
	switch_ldt_in_gdt();
}

