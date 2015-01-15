all:
	gcc elfextract.c -o elfextract
	gcc -c -m32 -o kernel.o kernel.c
	gcc -c -m32 -o interupt.o interupt.c
	nasm -f elf -o lib.o lib.asm
	nasm -f elf -o interupt_entry.o interupt_entry.asm
	ld -s -Ttext 0x8000 -o kernel.elf -melf_i386 kernel.o interupt.o lib.o interupt_entry.o
	./elfextract kernel.elf kernel.bin
	nasm -o loader.bin loader.asm
	dd if=loader.bin of=a.img conv=notrunc
	dd if=kernel.bin of=a.img bs=512 seek=1 conv=notrunc
	bochs

.PHONY: all
