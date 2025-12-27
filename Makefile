CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -Ikernel
LDFLAGS = -m elf_i386

OBJS = boot.o kernel.o memory.o

all: kernel.bin

boot.o:
	$(AS) -f elf32 boot/boot.asm -o boot.o

kernel.o:
	$(CC) $(CFLAGS) -c kernel/kernel.c -o kernel.o

memory.o:
	$(CC) $(CFLAGS) -c kernel/memory.c -o memory.o

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -T linker.ld -o kernel.bin $(OBJS)

run: kernel.bin
	qemu-system-i386 -kernel kernel.bin

clean:
	rm -f *.o kernel.bin
