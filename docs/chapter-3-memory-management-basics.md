চল, এবার আমরা MiniOS-এর **সবচেয়ে গুরুত্বপূর্ণ অধ্যায়গুলোর একটা**তে ঢুকি।
এটা সেই জায়গা যেখানে OS আর “toy” থাকে না,
এখান থেকেই এটা **systems project** হয়ে ওঠে।

নিচেরটা তুমি সরাসরি সেভ করতে পারো:

👉 `docs/chapter-3-memory-management-basics.md`

আগের মতোই: ধীরে, visualize করে, zero shortcut।

---

```markdown
# Chapter 3: Memory Management Basics
## How the Kernel Thinks About Memory

> Goal:
> বোঝা যে kernel কীভাবে memory কে দেখে,
> কীভাবে memory দেয়,
> আর কেন ভুল memory handling পুরো system ধ্বংস করতে পারে।

এই chapter শেষ হলে:
> malloc/free আর magic লাগবে না।

---

## 3.1 Memory Management কেন kernel-এর দায়িত্ব?

User program ভাবে:
> “আমাকে memory দাও”

Kernel ভাবে:
> “এই memory কার, কতক্ষণ, আর কখন ফেরত আসবে?”

Kernel ছাড়া কেউ জানে না:
- RAM কতটুকু
- কোনটা free
- কোনটা dangerous

তাই memory management kernel-এর **core duty**।

---

## 3.2 RAM: আবার byte array এ ফিরে যাই

Chapter 0 থেকে একটা কথা মনে রাখো:

> Memory = বিশাল byte array

Visual:

```

Address → 0        1        2        3        ...
RAM     → [  ][  ][  ][  ][  ][  ][  ][  ]

```

Kernel-এর কাজ:
- এই byte array কে ভাগ করা
- নিয়ম বানানো
- chaos আটকানো

---

## 3.3 Stack vs Heap (Kernel Perspective)

এই দুইটা শব্দ অনেকবার শোনা।
কিন্তু kernel দৃষ্টিতে দেখি।

---

### Stack কী?

Stack হলো:
> automatic, structured memory

Features:
- function call এ বাড়ে
- function return এ কমে
- CPU নিজে manage করে

Mental model:

```

High address
+-----------+
| function  |
| variables |
+-----------+
↓ grows down

```

Kernel early boot এ stack ছাড়া কিছুই সম্ভব না।

---

### Heap কী?

Heap হলো:
> dynamic, manually managed memory

Features:
- kernel নিজে দেয়
- kernel নিজে ফেরত নেয়
- orderless (theoretically)

Mental model:

```

Low address
+-----------+-----------+-----------+
|  used     |   used    |   free    |
+-----------+-----------+-----------+

```

Kernel allocator এখানেই কাজ করে।

---

## 3.4 Kernel Heap কেন আলাদা?

User heap:
- protected
- limited damage

Kernel heap:
- shared
- unprotected
- single bug = full crash

Mental rule:
> Kernel heap errors are fatal.

---

## 3.5 The Simplest Allocator: Bump Allocator

MiniOS শুরু করে সবচেয়ে simple allocator দিয়ে।

Concept:
> একটা pointer সামনে এগোয়

Mental picture:

```

heap_start ---------------------- heap_end
↑
free_memory_pointer

```

Allocation মানে:
- pointer সামনে যায়
- আগের memory আর free না

---

## 3.6 kmalloc(): Allocation as Pointer Movement

Pseudo flow:

```

addr = free_pointer
free_pointer += size
return addr

```

Visual:

```

Before:
[ used ][ free ][ free ][ free ]

After kmalloc(2 blocks):
[ used ][ used ][ free ][ free ]

```

Simple, fast, predictable।

---

## 3.7 Alignment: CPU-এর সাথে চুক্তি

CPU চায়:
> data নির্দিষ্ট boundary তে থাকুক

Alignment ছাড়া:
- performance degrade
- crash
- undefined behavior

Mental picture:

```

Allowed:
0x1000, 0x1004, 0x1008

Not allowed:
0x1001, 0x1002

```

Allocator এর দায়িত্ব:
> address ঠিক করে দেওয়া

---

## 3.8 Out Of Memory (OOM): Boundary Respect

Heap সীমা আছে।

Visual:

```

heap_start ---------------- heap_end
↑
allocation denied

```

OOM handling মানে:
- silent overwrite না
- controlled failure

Kernel rule:
> Failing loudly is safer than corrupting quietly.

---

## 3.9 Free(): Why It’s Harder Than It Looks

Allocation easy।
Free hard।

কারণ:
- memory reuse
- fragmentation
- ownership tracking

MiniOS এ আমরা শুরু করি:
> LIFO free (stack-like)

Mental model:

```

alloc A → alloc B → free B → free A

```

Anything else = danger.

---

## 3.10 Fragmentation: Invisible Enemy

Fragmentation মানে:
> memory আছে, কিন্তু usable না

Visual:

```

[ used ][ free ][ used ][ free ]

```

Total free memory plenty,
but no contiguous block.

এই জন্য advanced allocators complex।

---

## 3.11 Memory Stats: Knowing Yourself

Kernel যদি না জানে:
- কত memory used
- কত free

তাহলে debugging অন্ধ।

Stats mental model:

```

Used = free_ptr - heap_start
Free = heap_end - free_ptr

```

Observability kernel-এর জন্য luxury না,
survival tool।

---

## 3.12 Engineering Takeaway

এই chapter-এর core lesson:

> Memory management is about rules, not convenience.

- speed vs safety
- simplicity vs flexibility
- predictability > cleverness

Kernel প্রথমে predictable হয়,
তারপর smart।

---

## 3.13 Backend Bridge

Backend memory thinking:

- buffer lifecycle
- request-scoped memory
- pooling
- leak detection

সবগুলোর root:
> allocator mindset

Kernel এ যেটা fatal,
backend এ সেটা outage।

---

## 3.14 Chapter 3 Checklist

এই chapter শেষে তুমি পারবে:

- [ ] Memory কে byte array হিসেবে reason করতে
- [ ] Stack vs Heap kernel view explain করতে
- [ ] Bump allocator visualize করতে
- [ ] Alignment কেন দরকার বোঝাতে
- [ ] OOM কেন necessary বুঝতে
- [ ] Free কেন hard explain করতে

যদি এগুলো clear হয়:
👉 Phase 2 (Hands-on memory) তোমার জন্য safe

---

## Final Thought

Memory management মানে clever code না।
Memory management মানে:

> “আমি জানি এই byte এখন কার দায়িত্বে।”

এই প্রশ্নের উত্তর ভুল হলেই
system পড়ে যায়।
```

---

### একদম সৎ কথা ❤️

এই Chapter 3 মাথায় বসলে:

* Phase 2 তোমার কাছে আর ভয়ংকর লাগবে না
* allocator bug দেখলে panic কমবে
* backend memory issue ধরতে পারবে

চাও তো next আমরা করবো:

* **Phase 2: Hands-on Heap Implementation (Step-by-step)**
* বা Chapter 3 এর উপর **exercise sheet**
* বা **Advanced allocator intuition (without code)**

বল, পরের দরজাটা কোনটা খুলবো? 🚪
