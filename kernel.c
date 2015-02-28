#include "types.h"
#include "kernel.h"
#include "lib.h"
#include "interupt.h"
#include "protected.h"
#include "task.h"
#include "sys_call.h"
#include "time.h"

#define MEMCHK_NUM_ADDR	0x7e00
#define ADDR_RANGE_DESC_TBL_ADDR	(MEMCHK_NUM_ADDR+4)
#define GRAPHIC_ADDR	0xb800
#define PAGE_DIR_TBL_ADDR	0x100000
#define PAGE_TBL_ADDR	(PAGE_DIR_TBL_ADDR+0x1000)

int *tmp_dbg;
int ticks;

struct addr_range_desc {
	u32 baselow;
	u32 basehigh;
	u32 lenlow;
	u32 lenhigh;
	u32 type;
};

static void init_global_var();
static void get_gdt_info();
static int get_total_mem();
static int setup_paging();
/*
 * do not put any function defination upon cstart
 */
void cstart()
{
	/* do not put any function upon */
	init_global_var();
	get_gdt_info();
	init_pit();
	init_idtr();
	init_8259A();
	setup_idt();
	init_sys_call_tbl();

	init_tasks();

	set_interupt();
	start_task();
	while (1) {}
}




static void init_global_var()
{
	tmp_dbg = (int *)0x7dfc;
	k_reenter = -1;
	ticks = 0;
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
