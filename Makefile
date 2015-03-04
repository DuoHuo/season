all:
	gcc elfextract.c -o elfextract
	gcc -c -m32 -o kernel.o kernel.c
	gcc -c -m32 -o interupt.o interupt.c
	gcc -c -m32 -o libc.o libc.c -fno-stack-protector
	gcc -c -m32 -o protected.o protected.c
	gcc -c -m32 -o task.o task.c
	gcc -c -m32 -o sys_call.o sys_call.c
	gcc -c -m32 -o time.o time.c
	gcc -c -m32 -o keyboard.o keyboard.c
	gcc -c -m32 -o ulib.o ulib.c -fno-stack-protector
	nasm -f elf -o lib.o lib.asm
	nasm -f elf -o interupt_entry.o interupt_entry.asm
	nasm -f elf -o sys_call_entry.o sys_call_entry.asm
	nasm -f elf -o string.o string.asm
	ld -s -Ttext 0x8000 -o kernel.elf -melf_i386 kernel.o interupt.o lib.o interupt_entry.o libc.o protected.o task.o sys_call.o sys_call_entry.o time.o keyboard.o ulib.o string.o
	./elfextract kernel.elf kernel.bin
	nasm -o loader.bin loader.asm
	dd if=loader.bin of=a.img conv=notrunc
	dd if=kernel.bin of=a.img bs=512 seek=1 conv=notrunc
	bochs

.PHONY: all
