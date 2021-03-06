#ifndef LIB_H_H
#define LIB_H_H

#include "types.h"
#include "protected.h"
#include "interupt.h"

/* lib.asm */
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
extern void inc_char6();
extern void disp_str(char *string);

/* libc.c */
extern int disp_pos;	/* strores the current display position */

extern void *memcpy(void *dest, const void *src, int size);
extern void init_descriptor(struct descriptor *desc, u32 base, u32 limit, u16 attr);
extern char * kitoa(char * str, int num);
extern void disp_int(int integer);

#endif
