#ifndef PROTECTED_H_H
#define PROTECTED_H_H
#include "kernel.h"
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

#endif
