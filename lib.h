#ifndef LIB_H_H
#define LIB_H_H

#include "interupt.h"
#include "kernel.h"
#include "protected.h"

extern void apply_paging();
extern void out_byte(u16 port, u8 value);
extern u8 in_byte(u16 port);
extern void load_idtr(struct dtr idtr);
extern void load_gdtr(struct dtr gdtr);
extern void save_gdtr();
extern void load_ldtr(struct dtr ldtr);
extern void save_ldtr();
extern void set_interupt();
extern void clear_interupt();

#endif
