#ifndef LIBC_H_H
#define LIBC_H_H

#include "types.h"
#include "interupt.h"
#include "protected.h"

extern int disp_pos;

void *memcpy(void *dest, const void *src, int size);
void init_descriptor(struct descriptor *desc, u32 base, u32 limit, u16 attr);
extern char * itoa(char * str, int num);
extern void disp_int(int integer);
#endif
