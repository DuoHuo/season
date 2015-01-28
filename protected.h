#ifndef PROTECTED_H_H
#define PROTECTED_H_H
#include "kernel.h"

#define DA_32	0x4000
#define DA_LIMIT_4K	0x8000
#define DA_C	0x98	// existed code
#define DA_CR	0x9a	// existed code(r)
#define DA_DRW	0x92	// existed data(r/w)
#define DA_DR	0x90	// existed data(r)
#define DA_DRWA	0x93	// existed accessed data(r/2)
#define DA_LDT	0x82
#define DA_386TSS	0x89
#define DA_DPL0	0x00
#define DA_DPL1 0x20
#define DA_DPL2 0x40
#define DA_DPL3 0x60
#define	SA_RPL_MASK	0xFFFC
#define	SA_RPL0		0x0
#define	SA_RPL1		0x1
#define	SA_RPL2		0x2
#define	SA_RPL3		0x3
#define	SA_TI_MASK	0xFFFB
#define	SA_TIG		0
#define	SA_TIL		4

/* descriptor for GDT and LDT */
struct descriptor {
	u16 limit_low;
	u16 base_low;
	u8 base_mid;
	u8 attr1;
	u8 limit_high_attr2;
	u8 base_high;
};

typedef u16 selector_t;

struct idt_entry {
	u16 offset_low;
	u16 selector;
	u8 dcount;
	u8 attr;
	u16 offset_high;
};

/* gdtr, ldtr and idtr */
struct dtr {
	u16 limit;
	u32 base;
} __attribute__ ((packed));

extern void init_descriptor(struct descriptor *desc, u32 base, u32 limit, u16 attr);
#endif
