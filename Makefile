all:
	gcc elfextract.c -o elfextract
	gcc -c -m32 -o kernel.o kernel.c
	ld -s -Ttext 0x8000 -o kernel.elf kernel.o -melf_i386
	./elfextract kernel.elf kernel.bin
	nasm -o loader.bin loader.asm
	dd if=loader.bin of=a.img conv=notrunc
	dd if=kernel.bin of=a.img bs=512 seek=1 conv=notrunc
	bochs
.PHONY: all
