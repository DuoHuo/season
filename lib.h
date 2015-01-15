#ifndef LIB_H_H
#define LIB_H_H
#include "interupt.h"

extern void apply_paging();
extern void out_byte(u16 port, u8 value);
extern u8 in_byte(u16 port);
extern void load_idtr(struct idtr idtr_filler);
extern void set_interupt();
extern void clear_interupt();

#endif
