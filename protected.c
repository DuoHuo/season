#include "protected.h"
#include "kernel.h"

void init_descriptor(struct descriptor *desc, u32 base, u32 limit, u16 attr)
{
	desc->limit_low = limit & 0x0ffff;
	desc->base_low = base & 0x0ffff;
	desc->base_mid = (base >> 16) & 0x0ff;
	desc->attr1 = attr & 0xff;
	desc->limit_high_attr2 = ((limit >> 16) & 0x0f) | (attr >> 8) & 0xf0;
	desc->base_high = (base >> 24) & 0x0ff;
}
