#include "kernel.h"
#include "lib.h"
#include "interupt.h"
#include "interupt_entry.h"

struct idt_entry idt[IDT_SIZE];
struct dtr idtr;

void init_idtr()
{
	idtr.limit = IDT_SIZE*sizeof(struct idt_entry)-1;
	idtr.base = (u32)idt;
	load_idtr(idtr);
}

void init_idt_entry(unsigned char vector, int_handler handler, \
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

void setup_idt()
{
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
	out_byte(0x21,	0xfd);
	out_byte(0xa1,	0xff);
}

void spurious_irq(int irq)
{
	(*tmp_dbg) = 0xaa;
}

void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags)
{
	
}
