CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib
LDFLAGS = -m elf_i386

all: kernel.bin

boot.o:
	$(AS) -f elf32 boot/boot.asm -o boot.o

kernel.o:
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

kernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin boot.o kernel.o

run: kernel.bin
	qemu-system-x86_64 -kernel kernel.bin

clean:
	rm -f *.o kernel.bin
