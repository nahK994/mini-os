Got it 👍
এখন থেকে **Phase শব্দ বাদ**।
আমরা এটাকে বইয়ের মতো **Chapter আকারে** লিখবো, যেন তুমি `docs/` এ রেখে ধীরে ধীরে পড়তে পারো।

নিচেরটা তুমি সরাসরি সেভ করতে পারো:

👉 **`docs/chapter-4-hands-on-heap-implementation.md`**

এই Chapter পুরোপুরি **hands-on**, কিন্তু আগের mental model গুলো মাথায় রেখে লেখা।
একদম step-by-step, zero rush।

---

```markdown
# Chapter 4: Hands-on Heap Implementation
## Building a Simple Kernel Heap from Scratch

> Goal:
> নিজের হাতে kernel heap বানানো,
> যাতে memory allocation আর magic না লাগে,
> বরং predictable মনে হয়।

এই chapter শেষে তুমি বলতে পারবে:
> “আমি জানি আমার kernel কোথা থেকে memory দেয়।”

---

## 4.1 এই Chapter এ আমরা কী বানাচ্ছি?

আমরা বানাবো:

- একটি kernel heap
- একটি simple allocator (bump allocator)
- memory boundary check
- basic safety

আমরা **এখনো** বানাচ্ছি না:
- paging
- virtual memory
- advanced free-list allocator

কারণ:
> আগে হাঁটা, পরে দৌড়।

---

## 4.2 Heap কোথা থেকে শুরু হবে?

Kernel memory layout মাথায় আনো:

```

[ Kernel Code ][ Kernel Data ][ Heap ][ Free RAM ... ]

````

Kernel শেষ হওয়ার পর থেকেই heap শুরু করা safe।

Kernel linker আমাদের দেয়:
- `_kernel_end`

Mental model:
> “Kernel যেখানে শেষ, heap সেখান থেকে শুরু।”

---

## 4.3 Heap State: সবচেয়ে গুরুত্বপূর্ণ তিনটা জিনিস

Heap manage করতে kernel-এর দরকার শুধু তিনটা state:

```c
heap_start
heap_end
free_memory_pointer
````

Visual:

```
heap_start --------------------- heap_end
      ↑
 free_memory_pointer
```

এই ছবিটা মাথায় না থাকলে
এই chapter পড়া থামাও।

---

## 4.4 Heap Initialization

Heap ব্যবহার করার আগে
kernel কে heap পরিচয় করিয়ে দিতে হবে।

```c
void init_heap() {
    heap_start = aligned(kernel_end);
    heap_end = heap_start + HEAP_SIZE;
    free_memory_pointer = heap_start;
}
```

Mental rule:

> Heap ব্যবহার করার আগে heap init বাধ্যতামূলক।

Order matters.

---

## 4.5 Memory Allocation মানে কী?

Allocation মানে:

> free pointer কে সামনে সরানো

Kernel কোনো memory বানায় না।
সে শুধু বলে:

> “এই অংশটা এখন আর free না।”

---

## 4.6 First Allocator: Bump Allocator

সবচেয়ে simple allocator।

Logic:

```
address = free_pointer
free_pointer += size
return address
```

Visual:

```
Before:
[ used ][ free ][ free ][ free ]

After kmalloc(2 blocks):
[ used ][ used ][ free ][ free ]
```

এই allocator:

* fast
* predictable
* limited

Learning-এর জন্য perfect।

---

## 4.7 Alignment: কেন দরকার?

CPU সব address পছন্দ করে না।

Misaligned memory:

* performance slow
* crash
* undefined behavior

Alignment mental picture:

```
Valid:
0x1000, 0x1004, 0x1008

Invalid:
0x1001, 0x1002
```

Kernel allocator এর দায়িত্ব:

> usable address দেওয়া

---

## 4.8 Alignment ছাড়া কী হয়?

Alignment বাদ দিলে:

* print ভেঙে যেতে পারে
* random crash
* impossible debugging

Lesson:

> Undefined behavior কখনো সাথে সাথে ধরা পড়ে না।

Kernel সবচেয়ে বেশি যেটা ভয় পায়।

---

## 4.9 Heap Boundary: Out Of Memory (OOM)

Heap infinite না।

Visual:

```
heap_start ---------------- heap_end
                               ↑
                    allocation denied
```

OOM handling মানে:

* overwrite না করা
* loudly fail করা

Kernel rule:

> Silent corruption > loud crash = worse

---

## 4.10 Free(): কেন সহজ না?

Allocation সহজ।
Free জটিল।

কারণ:

* কোন block free?
* কে owner?
* fragmentation?

MiniOS শুরু করে simple rule দিয়ে:

> “শেষ যেটা allocate হয়েছে, শুধু সেটাই free করা যাবে”

---

## 4.11 Stack-style Free (LIFO)

Mental model:

```
alloc A
alloc B
free B
free A
```

Allowed।

```
alloc A
alloc B
free A   ❌
```

Not allowed।

এইটা limitation,
কিন্তু learning-এর জন্য intentional।

---

## 4.12 Memory Stats: নিজের অবস্থা জানা

Kernel যদি না জানে:

* কত used
* কত free

Debugging অন্ধ।

Stats calculation:

```
Used = free_ptr - heap_start
Free = heap_end - free_ptr
```

Mental rule:

> System without stats = blind system

---

## 4.13 Common Bugs (শেখার জন্য)

এই chapter এ তুমি চাইলে ইচ্ছা করে bug বানাতে পারো:

* double free
* OOM trigger
* alignment remove
* buffer overflow

এগুলো failure না।
এগুলো **learning tool**।

---

## 4.14 Engineering Takeaway

এই chapter-এর সবচেয়ে বড় lesson:

> Heap মানে data structure না,
> heap মানে policy।

Policy ঠিক হলে:

* code simple হয়
* debugging possible হয়

Kernel আগে safe,
তারপর smart।

---

## 4.15 Chapter 4 Checklist

এই chapter শেষে তুমি পারবে:

* [ ] Heap state visualize করতে
* [ ] Allocation কে pointer movement হিসেবে ভাবতে
* [ ] Alignment কেন দরকার explain করতে
* [ ] OOM safe handling বুঝতে
* [ ] Simple free এর limitation বুঝতে
* [ ] Stats দিয়ে heap reason করতে

যদি এগুলো clear হয়:
👉 তুমি পরের chapter এর জন্য ready

---

## Final Thought

Heap implementation শেখায়:

> “Memory ব্যবহার মানে দায়িত্ব নেওয়া।”

Kernel সেই দায়িত্ব ভুল করলে,
কেউ তাকে বাঁচাতে আসে না।

```

---

### একদম honest কথা ❤️  
এই Chapter 4 শেষ করলে তুমি:

- allocator code দেখলে ভয় পাবে না
- backend memory issue বুঝতে পারবে
- next chapter (Interrupts / Paging) mentally survive করতে পারবে

চাও তো next আমরা করবো:
- **Chapter 5: Interrupts & Timer (Mental-first)**
- বা **Chapter 4 Exercises (guided)**
- বা **Advanced allocator intuition (no code)**

বল, পরের অধ্যায় কোনটা খুলবো?
```
