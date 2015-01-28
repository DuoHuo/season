#include "kernel.h"
#include "lib.h"
#include "interupt.h"
#include "protected.h"

#define MEMCHK_NUM_ADDR	0x7e00
#define ADDR_RANGE_DESC_TBL_ADDR	(MEMCHK_NUM_ADDR+4)
#define GRAPHIC_ADDR	0xb800
#define PAGE_DIR_TBL_ADDR	0x100000
#define PAGE_TBL_ADDR	(PAGE_DIR_TBL_ADDR+0x1000)

#define NUM_TASKS	10
#define LDT_SIZE	4

int *tmp_dbg;
struct addr_range_desc {
	u32 baselow;
	u32 basehigh;
	u32 lenlow;
	u32 lenhigh;
	u32 type;
};

struct dtr gdtr;
struct dtr ldtr;


struct descriptor *gdt;

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

#define TASK_STACK_SIZE 200

struct tcb {
	struct regs regs;
	selector_t ldt_sel;
	struct descriptor ldt[LDT_SIZE];
	u32 pid;
	char p_name[16];
	u16 stack[TASK_STACK_SIZE];
};

struct tcb task_tbl[NUM_TASKS];

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

struct tss itss;
struct tcb *idle_task;

/*
 * do not put any function defination upon cstart
 */
static void init_global_var();
static void get_gdt_info();
static int get_total_mem();
static int setup_paging();


void idle_task_main();


void cstart()
{
	int ret;
	selector_t cs_sel;
	selector_t ds_sel;
	selector_t es_sel;
	selector_t fs_sel;
	selector_t ss_sel;
	selector_t gs_sel;

	/* do not put any function upon */
	init_global_var();
	get_gdt_info();
	if (ret < 0) {
		goto out;
	}
	init_idtr();
	init_8259A();
	setup_idt();
	set_interupt();

	idle_task = &task_tbl[0];
	/* initiate ldt in tcb */
	init_descriptor(&(idle_task->ldt[0]), 0, 0x0fffff, DA_32 | DA_LIMIT_4K | DA_C | DA_DPL1);
	init_descriptor(&(idle_task->ldt[1]), 0, 0x0fffff, DA_32 | DA_LIMIT_4K | DA_DRW| DA_DPL1);
	/* introduce ldt gdt */  //TODO careful, the gdt
	init_descriptor(&gdt[4], (u32)(idle_task->ldt), sizeof(struct descriptor) * LDT_SIZE - 1, DA_LDT);
	idle_task->ldt_sel = 32;
	/* init tss */
	itss.ss0 = 16; // Selector for Data
	init_descriptor(&gdt[5], (u32)&itss, sizeof(itss)-1, DA_386TSS);
	itss.iobase = sizeof(itss); // No I/O map
	clear_interupt();
	load_tss();
	/* initiate registers */
	cs_sel = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	ds_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	es_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	fs_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	ss_sel = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | SA_RPL1;
	gs_sel = (24 & SA_RPL_MASK) | SA_RPL1;
	idle_task->regs.cs = cs_sel;
	idle_task->regs.ds = ds_sel;
	idle_task->regs.es = es_sel;
	idle_task->regs.fs = fs_sel;
	idle_task->regs.ss = ss_sel;
	idle_task->regs.gs = gs_sel;
	idle_task->regs.eip = (u32)idle_task_main;
	idle_task->regs.esp = (u32) idle_task->stack + TASK_STACK_SIZE;
	idle_task->regs.eflags = 0x1202; // IF=0, IOPL=1, bit 2 is always 1

	start_idle();
out:
	for(;;) {};
}

static void init_global_var()
{
	tmp_dbg = (int *)0x7dfc; 
}

static void get_gdt_info()
{
	/* save gdt limit&base to gdtr */
	save_gdtr();
	gdt = (struct descriptor *)gdtr.base;
}

static int get_total_mem()
{
	struct addr_range_desc *ar;
	int *memchk_num;
	int total_mem = 0;
	int i;

	ar = (struct addr_range_desc *)ADDR_RANGE_DESC_TBL_ADDR;
	memchk_num = (int *)MEMCHK_NUM_ADDR;
	for (i=0; i<*memchk_num; i++) {
		total_mem += ar[i].lenlow;
	}
	return total_mem;
}

static int setup_paging()
{
	int total_mem;
	int page_tbl_num;
	int page_dir_tbl_num;
	pde_t *pde;
	pte_t *pte;
	long pt_base;
	long pte_base;
	int i;

	total_mem = get_total_mem();

	if (total_mem <= 0) {
		return ERR;
	}
	pde = (pde_t *)PAGE_DIR_TBL_ADDR;
	pte = (pte_t *)PAGE_TBL_ADDR;
	page_tbl_num = (total_mem+4095) / 4096;
	page_dir_tbl_num = (page_tbl_num+1023) / 1024;
	/* initiate page directory table */
	pt_base = PAGE_TBL_ADDR;
	for (i=0; i<page_dir_tbl_num; i++) {
		pde[i] = pt_base | 0x7;
		pt_base += 4096;
	}
	/* initiate page tables */
	pte_base = 0;
	for (i=0; i<page_tbl_num; i++) {
		pte[i] = pte_base | 0x7;
		pte_base += 4096;
	}
	apply_paging();
	return OK;
}

void idle_task_main()
{
	int i;

	for (i = 0; ; i++){
		(*tmp_dbg) = i;
	}
}
