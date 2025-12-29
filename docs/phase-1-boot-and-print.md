# MiniOS – Phase 1: Boot & Screen Output

Phase 1 হলো MiniOS-এর **জন্মপর্ব**।
এই ফেজে আমরা কোনো advanced feature বানাই না।
আমরা শুধু নিশ্চিত করি:

> “আমার OS সত্যিই boot হচ্ছে,  
> আর আমি screen-এ কিছু লিখতে পারছি।”

এই একটা কাজ ঠিকভাবে করতে পারলে,  
বাকি সব ধীরে ধীরে সম্ভব।

---

## Phase 1 এর লক্ষ্য

এই ফেজ শেষে MiniOS পারবে:

- QEMU তে boot হতে
- Kernel control নিতে
- Screen clear করতে
- Text print করতে
- Cursor এবং newline (`\n`) handle করতে

এই phase হলো Phase 2 (Memory) এর ভিত্তি।

---

## ধাপ ১: OS boot মানে কী?

### সাধারণ প্রোগ্রাম বনাম OS

Normal program:
- OS-এর উপর চলে
- `main()` দিয়ে শুরু

Operating System:
- নিজেই সবকিছুর দায়িত্ব নেয়
- কোনো OS নেই নিচে

তাই OS শুরু হয়:
> **bootloader দিয়ে**

---

## ধাপ ২: Bootloader এর কাজ

Bootloader:

- CPU কে 16-bit real mode থেকে বের করে
- Kernel কে memory তে load করে
- Kernel-এর entry point এ jump করে

MiniOS-এ আমরা:
- GRUB ব্যবহার করি
- নিজের bootloader লিখছি না (এখন)

---

## ধাপ ৩: Kernel entry point

Kernel-এর শুরুতে থাকে একটা function:

```c
void kernel_main() {
    // OS starts here
}
````

এই function থেকেই MiniOS সবকিছু নিয়ন্ত্রণ করে।

---

## ধাপ ৪: VGA Text Mode কী?

Graphics mode না,
আমরা ব্যবহার করি **VGA Text Mode**।

Screen layout:

* 80 columns × 25 rows
* প্রতি character = 2 byte

```
[ character ][ color ]
```

---

## ধাপ ৫: Screen memory (0xB8000)

`0xB8000` হলো RAM-এর একটা special জায়গা।

এই address এ লিখলে:

> screen-এ text দেখা যায়

---

### Screen memory access

```c
char* video_memory = (char*)0xB8000;
```

এখন এই pointer দিয়ে screen control করা যায়।

---

## ধাপ ৬: Simple print function

একটা basic print function:

```c
void print_char(char c, int index) {
    video_memory[index * 2] = c;
    video_memory[index * 2 + 1] = 0x0F;
}
```

`0x0F` মানে:

* White text
* Black background

---

## ধাপ ৭: Cursor management

Screen শুধু লিখলেই হয় না।
কোথায় লিখবো, সেটা track করতে হয়।

```c
int cursor_position = 0;
```

প্রতিটা character print করলে cursor সামনে যায়।

---

## ধাপ ৮: Newline (`\n`) support

`\n` মানে:

> “নতুন লাইনে যাও”

Implementation idea:

* বর্তমান row বের করো
* next row এর শুরুতে যাও

```c
if (c == '\n') {
    cursor_position += 80 - (cursor_position % 80);
}
```

---

## ধাপ ৯: Screen clear করা

Screen পরিষ্কার না করলে
আগের garbage দেখা যায়।

```c
void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = 0x0F;
    }
    cursor_position = 0;
}
```

---

## ধাপ ১০: “MiniOS is running…”

এটাই Phase 1-এর victory moment 🎉

```c
clear_screen();
print("MiniOS is running...\n");
```

এই লেখা দেখালে:

> Kernel সফলভাবে boot হয়েছে

---

## Phase 1 শেষে MiniOS কী শিখলো?

MiniOS এখন:

* Boot করতে পারে
* Kernel চালাতে পারে
* Screen control করতে পারে
* Text output দিতে পারে
* Cursor manage করতে পারে

সব future debugging এই screen দিয়েই হবে।

---

## Phase 1 কেন এত গুরুত্বপূর্ণ?

কারণ:

* Phase 2 memory invisible
* Phase 3 interrupts silent
* Phase 4 paging complex

Phase 1 হলো:

> “আমার OS বেঁচে আছে” এই প্রমাণ

---

## Backend ডেভেলপারদের জন্য শিক্ষা

Phase 1 শেখায়:

* abstraction-এর নিচে কী আছে
* debugging without tools
* minimal output importance
* systems thinking

এই phase skip করলে Phase 2 painful হয়।

---

## পরবর্তী ধাপ

👉 **Phase 2: Memory Management**

* Kernel end
* Heap
* kmalloc
* alignment
* OOM safety