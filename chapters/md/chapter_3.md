# অধ্যায় ৩

# C তে প্রবেশ (Getting to C)

এই অধ্যায়ে দেখানো হবে কীভাবে অপারেটিং সিস্টেম তৈরির জন্য অ্যাসেম্বলি ভাষার পরিবর্তে C ভাষা ব্যবহার করা যায়।

অ্যাসেম্বলি ভাষা CPU এর সাথে সরাসরি কাজ করার জন্য খুবই শক্তিশালী এবং কোডের প্রতিটি দিকের উপর সর্বোচ্চ নিয়ন্ত্রণ দেয়। তবে লেখকদের কাছে C একটি অনেক বেশি সুবিধাজনক ভাষা। তাই আমরা যতটা সম্ভব C ব্যবহার করতে চাই এবং শুধুমাত্র যেখানে প্রয়োজন সেখানে অ্যাসেম্বলি ব্যবহার করবো।

---

# স্ট্যাক সেটআপ করা (Setting Up a Stack)

C ব্যবহার করার একটি পূর্বশর্ত হলো একটি stack থাকা। কারণ প্রায় সব non-trivial C প্রোগ্রাম stack ব্যবহার করে।

x86 আর্কিটেকচারে stack নিম্ন ঠিকানার দিকে বৃদ্ধি পায়।

স্ট্যাক তৈরি করা কঠিন নয়। শুধু esp রেজিস্টারকে এমন একটি মুক্ত মেমরি অঞ্চলের শেষ ঠিকানায় নির্দেশ করতে হবে যা:

* ফ্রি
* সঠিকভাবে aligned (৪ বাইট alignment সুপারিশকৃত)

আমরা চাইলে esp কে যেকোনো র্যান্ডম মেমরি অঞ্চলে নির্দেশ করতে পারতাম। কিন্তু এটি ভালো ধারণা নয়, কারণ:

* কত মেমরি আছে তা জানা নেই
* সেটি অন্য কিছু দ্বারা ব্যবহৃত হতে পারে

এর চেয়ে ভালো পদ্ধতি হলো kernel এর ELF ফাইলের `.bss` সেকশনে uninitialized memory সংরক্ষণ করা।

`.bss` সেকশন ব্যবহার করার কারণ:

* executable ফাইলের সাইজ ছোট থাকে
* GRUB ELF বুঝতে পারে
* GRUB `.bss` এর জন্য মেমরি allocate করে দেয়

NASM pseudo-instruction `resb` ব্যবহার করে uninitialized memory ঘোষণা করা যায়:

```asm
KERNEL_STACK_SIZE equ 4096 ; stack এর সাইজ (বাইটে)

section .bss
align 4 ; ৪ বাইট alignment
kernel_stack: ; এই লেবেল মেমরির শুরু নির্দেশ করে

resb KERNEL_STACK_SIZE ; kernel এর জন্য stack সংরক্ষণ
```

Uninitialized memory নিয়ে চিন্তা করার দরকার নেই। কারণ সঠিক প্রোগ্রামে কখনও এমন stack লোকেশন থেকে pop করা সম্ভব নয় যেটিতে আগে push করা হয়নি।

স্ট্যাক পয়েন্টার সেট করা হবে এভাবে:

```asm
mov esp, kernel_stack + KERNEL_STACK_SIZE
```

এটি esp কে stack এর শুরুর জায়গায় (মেমরির শেষ প্রান্তে) নির্দেশ করে।

---

# অ্যাসেম্বলি থেকে C কোড কল করা (Calling C Code From Assembly)

অ্যাসেম্বলি থেকে C ফাংশন কল করার বিভিন্ন calling convention আছে। এই বইতে ব্যবহৃত হয়েছে **cdecl** calling convention, কারণ GCC এটি ব্যবহার করে।

cdecl নিয়ম:

* আর্গুমেন্ট stack এর মাধ্যমে পাঠানো হবে
* ডান থেকে বামে push করতে হবে
* রিটার্ন ভ্যালু থাকবে `eax` রেজিস্টারে

উদাহরণ:

### C ফাংশন:

```c
int sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}
```

### অ্যাসেম্বলি কোড:

```asm
extern sum_of_three

push dword 3 ; arg3
push dword 2 ; arg2
push dword 1 ; arg1
call sum_of_three
```

ফলাফল থাকবে `eax` রেজিস্টারে।

---

# স্ট্রাক্ট প্যাকিং (Packing Structs)

অনেক সময় “configuration bytes” ব্যবহার করতে হয় যেখানে বিটগুলো নির্দিষ্ট ক্রমে সাজানো থাকে।

উদাহরণ:

```
Bit:      | 31 - 24 | 23 - 8 | 7 - 0 |
Content:  | index   | address| config|
```

unsigned int ব্যবহার না করে packed struct ব্যবহার করা সুবিধাজনক:

```c
struct example {
    unsigned char config;    /* bit 0 - 7 */
    unsigned short address;  /* bit 8 - 23 */
    unsigned char index;     /* bit 24 - 31 */
};
```

কিন্তু সমস্যা হলো:

কম্পাইলার performance বা alignment এর জন্য padding যোগ করতে পারে।

এটি configuration struct এর জন্য বিপজ্জনক।

সমাধান:

```c
struct example {
    unsigned char config;
    unsigned short address;
    unsigned char index;
} __attribute__((packed));
```

⚠️ `__attribute__((packed))` C standard এর অংশ নয়। সব কম্পাইলারে কাজ নাও করতে পারে।

---

# C কোড কম্পাইল করা (Compiling C Code)

আমাদের OS এ কোনো standard library নেই।

তাই GCC কে বিশেষ flag দিয়ে কম্পাইল করতে হবে।

ব্যবহৃত flag:

```
-m32 
-nostdlib 
-nostdinc 
-fno-builtin 
-fno-stack-protector 
-nostartfiles 
-nodefaultlibs
```

সতর্কতা সক্রিয় রাখতে:

```
-Wall -Wextra -Werror
```

এখন `kmain.c` নামে একটি ফাইল তৈরি করে `kmain()` ফাংশন বানানো যাবে।

প্রথমে এটি কোনো আর্গুমেন্ট না নিলেও চলবে।

---

# বিল্ড টুলস (Build Tools)

make ব্যবহার করা উত্তম।

একটি সাধারণ Makefile উদাহরণ:

```make
OBJECTS = loader.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
	-b boot/grub/stage2_eltorito \
	-no-emul-boot \
	-boot-load-size 4 \
	-A os \
	-input-charset utf8 \
	-quiet \
	-boot-info-table \
	-o os.iso \
	iso

run: os.iso
	bochs -f bochsrc.txt -q

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso
```

---

# ডিরেক্টরি স্ট্রাকচার

```
.
|-- bochsrc.txt
|-- iso
|   |-- boot
|   |   |-- grub
|   |   |   |-- menu.lst
|   |   |   |-- stage2_eltorito
|-- kmain.c
|-- loader.s
|-- Makefile
```

OS চালাতে:

```
make run
```

---

# Further Reading

• Kernighan & Ritchie — *The C Programming Language (Second Edition)*

