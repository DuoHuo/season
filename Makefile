all:
	gcc elfextract.c -o elfextract
	gcc -c -m32 -o kernel.o kernel.c
	nasm -f elf -o lib.o lib.asm
	ld -s -Ttext 0x8000 -o kernel.elf -melf_i386 kernel.o lib.o
	./elfextract kernel.elf kernel.bin
	nasm -o loader.bin loader.asm
	dd if=loader.bin of=a.img conv=notrunc
	dd if=kernel.bin of=a.img bs=512 seek=1 conv=notrunc
	bochs

.PHONY: all
