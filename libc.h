#ifndef LIBC_H_H
#define LIBC_H_H

#include "types.h"
#include "interupt.h"

void *memcpy(void *dest, const void *src, int size);
void init_descriptor(struct descriptor *desc, u32 base, u32 limit, u16 attr);
#endif
