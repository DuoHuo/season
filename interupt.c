#include "types.h"
#include "kernel.h"
#include "interupt.h"
#include "interupt_entry.h"
#include "protected.h"
#include "task.h"
#include "lib.h"
#include "keyboard.h"

int_handler isr_tbl[ISR_NUM];

int k_reenter; /* detect reentering of interupt handler */


static void init_isr_tbl();
static void init_idt_entry(unsigned char vector, int_handler handler, \
			unsigned char privilege);
extern void sys_call();

void init_idtr()
{
	idtr.limit = IDT_SIZE*sizeof(struct idt_entry)-1;
	idtr.base = (u32)idt;
	load_idtr(idtr);
}

static void init_idt_entries()
{
	/* error interupt */
	init_idt_entry(0x00, divide_error, PRIVILEGE_KERN);
	init_idt_entry(0x01, single_step_exception, PRIVILEGE_KERN);
	init_idt_entry(0x02, nmi, PRIVILEGE_KERN);
	init_idt_entry(0x03, breakpoint_exception, PRIVILEGE_USER);
	init_idt_entry(0x04, overflow, PRIVILEGE_USER);
	init_idt_entry(0x05, bounds_check, PRIVILEGE_KERN);
	init_idt_entry(0x06, inval_opcode, PRIVILEGE_KERN);
	init_idt_entry(0x07, copr_not_available, PRIVILEGE_KERN);
	init_idt_entry(0x08, double_fault, PRIVILEGE_KERN);
	init_idt_entry(0x09, copr_seg_overrun, PRIVILEGE_KERN);
	init_idt_entry(0x0a, inval_tss, PRIVILEGE_KERN);
	init_idt_entry(0x0b, segment_not_present, PRIVILEGE_KERN);
	init_idt_entry(0x0c, stack_exception, PRIVILEGE_KERN);
	init_idt_entry(0x0d, general_protection, PRIVILEGE_KERN);
	init_idt_entry(0x0e, page_fault, PRIVILEGE_KERN);
	init_idt_entry(0x10, copr_error, PRIVILEGE_KERN);
	/* hardware interupt */
	init_idt_entry(0x20, hwint00, PRIVILEGE_KERN);
	init_idt_entry(0x21, hwint01, PRIVILEGE_KERN);
	init_idt_entry(0x22, hwint02, PRIVILEGE_KERN);
	init_idt_entry(0x23, hwint03, PRIVILEGE_KERN);
	init_idt_entry(0x24, hwint04, PRIVILEGE_KERN);
	init_idt_entry(0x25, hwint05, PRIVILEGE_KERN);
	init_idt_entry(0x26, hwint06, PRIVILEGE_KERN);
	init_idt_entry(0x27, hwint07, PRIVILEGE_KERN);
	init_idt_entry(0x28, hwint08, PRIVILEGE_KERN);
	init_idt_entry(0x29, hwint09, PRIVILEGE_KERN);
	init_idt_entry(0x2a, hwint10, PRIVILEGE_KERN);
	init_idt_entry(0x2b, hwint11, PRIVILEGE_KERN);
	init_idt_entry(0x2c, hwint12, PRIVILEGE_KERN);
	init_idt_entry(0x2d, hwint13, PRIVILEGE_KERN);
	init_idt_entry(0x2e, hwint14, PRIVILEGE_KERN);
	init_idt_entry(0x2f, hwint15, PRIVILEGE_KERN);
	/* system call */
	init_idt_entry(0x80, sys_call, PRIVILEGE_USER);
}

void setup_idt()
{
	init_idt_entries();
	init_isr_tbl();
}

void init_8259A()
{
	out_byte(0x20,	0x11);
	out_byte(0xa0,	0x11);
	out_byte(0x21,	0x20);
	out_byte(0xa1,	0x28);
	out_byte(0x21,	0x4);
	out_byte(0xa1,	0x2);
	out_byte(0x21,	0x1);
	out_byte(0xa1,	0x1);
	out_byte(0x21,	0xfc);
	out_byte(0xa1,	0xff);
}


void switch_ldt_in_gdt()
{

	init_descriptor(&gdt[4], (u32)(ready_task->ldt), sizeof(struct descriptor) * LDT_SIZE - 1, DA_LDT);
}

void add_isr(int irq, int_handler handler)
{
	isr_tbl[irq] = handler;
}

/* Below are real ISRs */

static void clock_routine(int irq)
{
	ticks ++;
	scheduler();
}

static void keyboard_routine(int irq)
{
	u8 code;

	code = in_byte(0x60);
	if (keyboard_buf.count < KEYBOARD_BUF_LEN) {
		*(keyboard_buf.head) = code;
		keyboard_buf.head++;
		if (keyboard_buf.head == keyboard_buf.buf + KEYBOARD_BUF_LEN)
			keyboard_buf.head = keyboard_buf.buf;
		keyboard_buf.count++;
	}
}

static void default_routine(int irq)
{
}

void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags)
{
	int i;

	char * err_msg[] = {"#DE Divide Error",
			    "#DB RESERVED",
			    "--  NMI Interrupt",
			    "#BP Breakpoint",
			    "#OF Overflow",
			    "#BR BOUND Range Exceeded",
			    "#UD Invalid Opcode (Undefined Opcode)",
			    "#NM Device Not Available (No Math Coprocessor)",
			    "#DF Double Fault",
			    "    Coprocessor Segment Overrun (reserved)",
			    "#TS Invalid TSS",
			    "#NP Segment Not Present",
			    "#SS Stack Segment Fault",
			    "#GP General Protection",
			    "#PF Page Fault",
			    "--  (Intel reserved. Do not use.)",
			    "#MF x87 FPU Floating Point Error (Math Fault)",
			    "#AC Aligment Check",
			    "#MC Machine Check",
			    "#XF SIMD Floating Point Exception"
	};

	disp_pos = 0;
	for (i=0; i<80*5; i++) {
		disp_str(" ");
	}
	disp_pos = 0;

	disp_str("Exception! --> ");
	disp_str(err_msg[vec_no]);
	disp_str("\n\n");
	disp_str("EFLAGS:");
	disp_int(eflags);
	disp_str("  CS:");
	disp_int(cs);
	disp_str("  EIP:");
	disp_int(eip);

	if (err_code != 0xffffffff) {
		disp_str("  Error code:");
		disp_int(err_code);
	}
}


static void init_isr_tbl()
{
	int i;

	for (i = 0; i < ISR_NUM; i++) {
		isr_tbl[i] = default_routine;
	}
	add_isr(0, clock_routine);
	add_isr(1, keyboard_routine);
}

/*
 * init_idt_entry is used to initiate idt entries, using add_isr function 
 * to define specific Interupt Service Routine(ISR)
**/
static void init_idt_entry(unsigned char vector, int_handler handler, \
			unsigned char privilege)
{
	struct idt_entry *entry = &idt[vector];
	u32 base = (u32)handler;

	entry->offset_low = base & 0xffff;
	entry->selector = 8; //TODO
	entry->dcount = 0;
	entry->attr = 0x8E | (privilege << 5);
	entry->offset_high = (base >> 16) & 0xffff;
}

