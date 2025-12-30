# Chapter 0: Engineering Grounding (Mental Model)
## Think Like a Systems Engineer (Before Writing Code)

> Goal:
> Code লেখার আগে system কিভাবে ভাবে সেটা শেখা।
> এই chapter শেষ করলে তুমি আর blindly code লিখবে না।

---

## 0.1 Program vs Process vs Kernel

এই তিনটা শব্দ একসাথে শুনলেই অনেকের মাথা ঘুরে যায়।
চল, একদম আলাদা করে দেখি।

---

### Program কী?

Program হলো:
> Disk এ পড়ে থাকা একটা **নিষ্প্রাণ ফাইল**

উদাহরণ:
- `a.out`
- `kernel.bin`
- `server`

Mental picture:
```

Disk:
[ program file ]   ← nothing is running

```

Program নিজে নিজে কিছু করে না।
এটা শুধু instructions + data।

---

### Process কী?

Process হলো:
> Program যখন **চলমান অবস্থায় RAM এ থাকে**

একই program থেকে একাধিক process হতে পারে।

Mental picture:
```

RAM:
[ code ][ stack ][ heap ][ registers ]

```

Important:
- Process এর নিজের memory state থাকে
- OS process তৈরি করে, চালায়, থামায়

---

### Kernel কী?

Kernel হলো:
> সবচেয়ে বিশেষ process  
> যে অন্য সব process কে নিয়ন্ত্রণ করে

Kernel:
- memory কে ভাগ করে
- CPU সময় দেয়
- process kill করতে পারে

Mental picture:
```

CPU
├── Kernel (boss)
├── Process A
├── Process B

```

Kernel নিজে:
- user process এর মতো না
- ভুল করলে পুরো system মরতে পারে

এই জন্য kernel code ভয় পায় **undefined behavior** কে।

---

## 0.2 Memory Is Just a Byte Array

এই ধারণাটা system engineering-এর backbone।

Memory আসলে:
> বিশাল একটা byte array

Visualize করো:

```

Address →   0   1   2   3   4   5   6   7
Memory  → [ A ][ B ][ C ][ D ][ E ][ F ][ G ][ H ]

```

কিছুই special না।
সবই byte।

---

### Data structure আসলে কী?

`int`, `struct`, `array`  
সবই আসলে byte গুলোর ব্যাখ্যা।

Example:
```

int x = 10;

Memory:
[0A][00][00][00]   (little endian)

````

Mental rule:
> CPU byte দেখে না, তুমি কী বোঝাতে চাও সেটা দেখে

ভুল বোঝালে CPU তোমাকে বাঁচাবে না।

---

## 0.3 Pointer: Address, Not Magic

Pointer মানে:
> Memory array-এর একটা index

Example:
```c
int x = 10;
int* p = &x;
````

Mental model:

```
Memory:
Address 1000 → [ 0A ][ 00 ][ 00 ][ 00 ]

p = 1000
```

Pointer:

* data না
* ownership না
* শুধু address

---

### Pointer vs Ownership (খুব গুরুত্বপূর্ণ)

Pointer উত্তর দেয়:

> “কোথায়?”

Ownership উত্তর দেয়:

> “কে দায়ী?”

Kernel-level thinking:

* কে allocate করলো?
* কে ব্যবহার করছে?
* কে free করবে?

Ownership ভুল হলে system ধীরে ধীরে মারা যায়।

---

## 0.4 Dangling Pointer কী?

Dangling pointer হলো:

> Pointer আছে, কিন্তু যেটাকে দেখাচ্ছে সেটা আর valid না

Example:

```c
char* p = kmalloc(100);
kfree();
*p = 'A';   // ❌
```

Mental picture:

```
Before free:
p → [ valid memory ]

After free:
p → [ unknown / reused memory ]
```

Danger:

* silent corruption
* random crash
* impossible debugging

---

## 0.5 Undefined Behavior (UB) কী?

Undefined behavior মানে:

> “এখানে কী হবে আমরা guarantee দিচ্ছি না”

Kernel ভয় পায় UB কে কারণ:

* কোনো warning দেয় না
* কোনো error দেয় না
* system silently corrupt হয়

Example UB:

* use after free
* buffer overflow
* unaligned access
* invalid pointer dereference

---

### কেন kernel এ UB ভয়ংকর?

User program crash করলে:
→ process মারা যায়

Kernel crash করলে:
→ **সব মারা যায়**

Mental rule:

> Kernel trusts you. Blindly.

---

## 0.6 Reasoning About Memory Without Tools

এই chapter এর সবচেয়ে বড় skill।

তুমি debugger ছাড়াই বলতে পারবে:

* pointer কোথায় যাচ্ছে
* memory state কেমন
* কোন line dangerous

Example reasoning:

```
heap_start → free_ptr → heap_end

kmalloc(100):
free_ptr moves forward

kfree():
free_ptr moves backward
```

এটাই systems thinking।

---

## 0.7 Common Beginner Illusions

❌ “C আমাকে protect করবে”
❌ “Compiler error দেবে”
❌ “It worked once, so correct”

✔ Reality:

> Kernel programming = responsibility, not safety

---

## 0.8 Chapter 0 Checkpoints

এই chapter শেষ হলে তুমি পারবে:

* [ ] Program, Process, Kernel আলাদা করে বোঝাতে
* [ ] Memory কে byte array হিসেবে কল্পনা করতে
* [ ] Pointer আর ownership আলাদা করতে
* [ ] Dangling pointer কেন ভয়ংকর explain করতে
* [ ] Undefined behavior কেন kernel-এর শত্রু বুঝতে
* [ ] Tool ছাড়া memory state reason করতে

যদি এগুলো পারো:
👉 তুমি Chapter 1-এর জন্য ready

---

## Final Note

এই chapter কোড শেখায় না।
এই chapter **ভুল কম করতে শেখায়**।

ভাল systems engineer হওয়ার সবচেয়ে বড় skill:

> আগে ভাবা, পরে লেখা