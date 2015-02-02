#ifndef LIB_H_H
#define LIB_H_H

#include "types.h"
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
extern void load_tss();
extern void start_task();
extern void inc_char2();
extern void inc_char4();

#endif
