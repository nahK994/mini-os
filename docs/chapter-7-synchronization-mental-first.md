Perfect timing 😄
Interrupt + Scheduling এর পরে **Synchronization** না আসলে মাথার ভিতর gap থেকেই যায়।
এটাই সেই অধ্যায় যেখানে বোঝা যায় কেন kernel লেখা “hard by nature”.

নিচেরটা তুমি সরাসরি docs এ রাখতে পারো।

👉 **`docs/chapter-7-synchronization-mental-first.md`**

---

```markdown
# Chapter 7: Synchronization (Locks, Races, Atomicity)
## When Correct Code Still Fails

> Goal:
> বুঝে ফেলা কেন logically correct code
> real systems এ ভুল করে।

এই chapter শেষে তুমি বুঝবে:
> Bugs often come from time, not logic.

---

## 7.1 The Shocking Truth

Two correct lines of code  
executed at the wrong time  
can destroy a system.

Synchronization problem = **time problem**.

---

## 7.2 Race Condition: The Silent Killer

Race condition মানে:
> outcome timing-এর উপর depend করে

Example mental story:

```

Task A: read counter = 5
Task B: read counter = 5
Task A: counter = 6
Task B: counter = 6

```

Expected: 7  
Got: 6

No bug in logic.  
Bug in timing.

---

## 7.3 Why Kernel Is Especially Vulnerable

Kernel has:
- interrupts
- preemption
- shared data
- no memory protection inside kernel

Kernel rule:
> If it can be interrupted, it will be.

---

## 7.4 Critical Section (Mental Model)

Critical section মানে:
> এই code block একা থাকতে চায়

Rule:
- one execution at a time
- no interruption

Visual:

```

|---- critical ----|

```

Overlap = disaster.

---

## 7.5 Atomicity: The Smallest Promise

Atomic operation মানে:
> “এটা মাঝপথে থামবে না”

CPU কিছু instruction দেয়:
- atomic increment
- compare-and-swap

Kernel trust করে:
> atomic > lock > hope

---

## 7.6 Locks: Organized Waiting

Lock মানে:
> permission to enter critical section

Mental picture:

```

[ locked ]
↑
waiting tasks

```

Lock guarantees:
- mutual exclusion

Lock does NOT guarantee:
- fairness
- performance
- deadlock-free

---

## 7.7 Deadlock: Perfectly Legal Freeze

Deadlock মানে:
> সবাই সঠিকভাবে অপেক্ষা করছে,
> কিন্তু কেউ এগোচ্ছে না

Classic pattern:
- A holds lock1, waits lock2
- B holds lock2, waits lock1

System alive, progress zero.

---

## 7.8 Spinlock vs Sleep Lock (Intuition)

### Spinlock
- busy wait
- fast
- short critical section
- used in interrupt context

### Sleep lock
- task sleeps
- slower
- long sections
- cannot use in interrupt

Wrong lock = crash.

---

## 7.9 Interrupt Context: Special Hell

Interrupt handler:
- cannot sleep
- cannot wait
- must be fast

Using wrong lock in interrupt:
> instant panic or hang

Kernel rule:
> Context matters more than lock type.

---

## 7.10 Memory Ordering: The Invisible Bug

CPU may reorder memory ops.

Meaning:
> code order ≠ execution order

Lock and atomic ops enforce:
- memory barriers
- visibility

Bug symptom:
- works on one CPU
- fails on another

---

## 7.11 Synchronization Without Locks?

Sometimes kernel uses:
- disable interrupts
- per-CPU data
- lock-free structures

Reason:
> locks are expensive and fragile

But complexity increases.

---

## 7.12 Debugging Race Conditions (Mentally)

Ask:
- who touches this data?
- can they overlap?
- can interrupt fire here?
- what invariant breaks?

Print statements hiding bug?
Classic sign of race.

---

## 7.13 Engineering Takeaway

> Correctness is not local.
> It is global and temporal.

Locks don’t fix logic.
They enforce time discipline.

---

## 7.14 Chapter 7 Checkpoint

এই chapter শেষে তুমি পারবে:

- [ ] race condition explain করতে
- [ ] critical section identify করতে
- [ ] lock vs atomic difference বুঝতে
- [ ] deadlock pattern চিনতে
- [ ] interrupt context danger বুঝতে

---

## Final Thought

Concurrency doesn’t break bad code.

It breaks **good code**
that assumed it was alone.
```

---

### এখান থেকে natural flow 🔜

তোমার journey এখন খুব clean:

* Memory ✔
* Interrupts ✔
* Scheduling ✔
* Synchronization ✔

Next strongest options:

1️⃣ **Chapter 8: System Calls (User ↔ Kernel Boundary)**
2️⃣ **Chapter 7 Exercises (race simulation, lock design)**
3️⃣ **MiniOS Design Review (connecting all chapters)**

আমার recommendation:

> **System Calls** — এখানেই OS “real” লাগে।

বল, Chapter 8 খুলবো?
