Perfect.
Chapter 0 যদি ছিল **“কিভাবে ভাবতে হবে”**,
তাহলে **Chapter 1 হলো “কিভাবে জীবন শুরু হয়”**।

নিচেরটা তুমি সরাসরি সেভ করতে পারো:

👉 `docs/chapter-1-boot-and-kernel-skeleton.md`

আমি এখানেও **shortcut নেইনি**, ধীরে, visualize করে লিখেছি।
এই অধ্যায় বুঝলে “OS magically starts” ভাবনা শেষ হবে।

---

```markdown
# Chapter 1: Boot & Kernel Skeleton
## How Code Starts Running on Bare Metal

> Goal:
> বোঝা যে কম্পিউটার চালু হওয়ার পর
> **প্রথম instruction কোথা থেকে আসে**
> এবং কিভাবে সেটা kernel পর্যন্ত পৌঁছায়।

এই অধ্যায় শেষে boot process আর magic লাগবে না।

---

## 1.1 Power On: CPU Reset State

কম্পিউটার চালু করলে প্রথম যে জিনিসটা জাগে:
> **CPU**

কোনো OS না  
কোনো kernel না  
কোনো memory manager না  

শুধু CPU + RAM + firmware

---

### CPU reset হলে কী অবস্থা থাকে?

CPU reset state মানে:
- নির্দিষ্ট register value
- নির্দিষ্ট instruction pointer
- খুব সীমিত ক্ষমতা

Mental picture:

```

Power ON
↓
CPU reset
↓
CPU জানে শুধু:
"I must start from a fixed address"

```

এই address সাধারণত firmware ঠিক করে দেয়।

---

## 1.2 CPU একা কিছু করতে পারে না

CPU নিজে জানে না:
- disk কোথায়
- OS কোথায়
- kernel কী

CPU শুধু পারে:
> instruction fetch → execute

তাই প্রশ্ন আসে:
> “কে CPU কে প্রথম instruction দেবে?”

---

## 1.3 Bootloader এর দায়িত্ব

Bootloader হলো:
> CPU আর kernel-এর মাঝখানের সেতু 🌉

Responsibility:
1. CPU কে usable অবস্থায় আনা
2. Kernel memory তে লোড করা
3. Kernel-এর entry point এ jump করা

Mental picture:

```

CPU → Bootloader → Kernel

```

Bootloader নিজে OS না।
এটা শুধু **starter motor**।

---

## 1.4 Bootloader কী কী কাজ করে?

Minimum কাজ:

- CPU mode সেট করা
- Memory layout prepare করা
- Kernel কোথায় আছে সেটা জানা
- Kernel-এ control হস্তান্তর

Bootloader ভুল করলে:
→ kernel কখনোই চলবে না

এই জন্য bootloader code:
- ছোট
- স্পষ্ট
- boring হওয়া ভালো

---

## 1.5 Real Mode vs Protected Mode

এখানে একটু ধীরে পড়ো।
এই অংশটা অনেক জায়গায় ভয় লাগায়।

---

### Real Mode কী?

Real Mode হলো:
> CPU-এর সবচেয়ে পুরোনো, সবচেয়ে simple mode

Features:
- 16-bit
- 1MB memory access
- কোনো protection নেই

Mental model:
```

CPU with training wheels

```

Legacy কারণে CPU এখান থেকে শুরু করে।

---

### Protected Mode কী?

Protected Mode হলো:
> আধুনিক CPU operation mode

Features:
- 32-bit বা 64-bit
- virtual memory
- memory protection
- privilege levels

Mental model:
```

CPU without training wheels

```

Kernel এখানে থাকতে চায়।

---

### কেন mode switch দরকার?

CPU power on → Real Mode  
OS needs → Protected Mode

So:
> Bootloader এর কাজ CPU কে upgrade করা

---

## 1.6 Jumping Into the Kernel

সব প্রস্তুতি শেষে সবচেয়ে গুরুত্বপূর্ণ মুহূর্ত:

> **Control transfer**

Bootloader বলে:
> “এখন থেকে তুমি দায়িত্বে”

Technical ভাবে:
- instruction pointer kernel entry address এ set হয়
- stack initialize হয়
- CPU kernel code execute শুরু করে

Mental picture:

```

Bootloader running
↓
Jump to kernel entry
↓
Kernel starts executing

```

এই jump ভুল হলে:
- triple fault
- reboot
- blank screen

---

## 1.7 Kernel Skeleton মানে কী?

Kernel skeleton মানে:
> Kernel-এর সবচেয়ে minimal structure

এটা কোনো feature না।
এটা শুধু proof:

> “Kernel code is alive”

Example responsibility:
- basic setup
- print something
- infinite loop

Mental model:
```

Kernel entry
↓
Initialize basics
↓
"I'm alive"
↓
while(1)

```

Elegance এখানে দরকার নেই।
Correctness দরকার।

---

## 1.8 Initialization Order কেন এত গুরুত্বপূর্ণ?

এই অধ্যায়ের সবচেয়ে গুরুত্বপূর্ণ lesson 👇

> **Initialization order matters more than code elegance**

কারণ:
- stack না থাকলে function call ভাঙবে
- memory setup না হলে allocation ভাঙবে
- interrupt setup না হলে random crash

Kernel development rule:
> Wrong order = silent death

---

### Example wrong order

❌ Print before VGA setup  
❌ Allocate before heap init  
❌ Enable interrupt before handler

সবই crash, কিন্তু explain নেই।

---

## 1.9 Engineering Takeaway

এই অধ্যায়ের takeaway এক লাইনে:

> Kernel engineering হলো
> **ঠিক সময়ে ঠিক জিনিস initialize করা**

সুন্দর code পরে।
ঠিক order আগে।

---

## 1.10 Mental Checklist (Chapter 1)

এই chapter শেষে তুমি পারবে:

- [ ] CPU reset state explain করতে
- [ ] Bootloader এর দায়িত্ব বলতে
- [ ] Real vs Protected mode আলাদা করতে
- [ ] Kernel entry মানে কী বুঝতে
- [ ] Initialization order কেন critical বুঝতে

যদি এগুলো clear হয়:
👉 তুমি Chapter 2 এর জন্য mentally ready

---

## Final Thought

Kernel শুরু হয় কোনো library দিয়ে না,
কোনো framework দিয়ে না।

Kernel শুরু হয়:
> “Nothing → Something”

এই transition বুঝলেই
OS আর ভয়ংকর লাগে না।
```

---

### Honest engineering note ❤️

এই Chapter 1 ভালোভাবে মাথায় বসলে:

* bootloader code ছোট লাগবে
* kernel crash হলে panic কম হবে
* তুমি বুঝবে “এখানে কিছু এখনো init হয়নি”

চাও তো next আমরা করবো:

* **Chapter 2: Text Output & Hardware IO**
* বা Chapter 1 এর উপর **diagram-only recap**
* বা **Boot → Backend server analogy**

বল, পরের অধ্যায় কোনটা? 🚀
