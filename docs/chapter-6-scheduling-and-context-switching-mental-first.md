একদম ঠিক জায়গায় এসেছো 🙂
Interrupt বোঝার পর **Scheduling** naturally আসে।
এটা সেই অধ্যায় যেখানে OS আর “library” থাকে না, **living system** হয়ে যায়।

নিচেরটা তুমি সরাসরি docs এ রাখতে পারো।

👉 **`docs/chapter-6-scheduling-and-context-switching-mental-first.md`**

---

```markdown
# Chapter 6: Scheduling & Context Switching (Mental-first)
## How One CPU Pretends to Be Many

> Goal:
> বুঝে ফেলা CPU কিভাবে একসাথে অনেক কাজ করছে বলে মনে করায়,
> অথচ বাস্তবে একবারে একটা কাজই করে।

এই chapter শেষে তুমি বুঝবে:
> Multitasking is an illusion created by discipline and timing.

---

## 6.1 The Fundamental Lie: “Everything Runs Together”

User ভাবে:
- browser চলছে
- music চলছে
- terminal চলছে

CPU reality:
> এক সময় একটাই instruction execute হয়।

OS কাজ:
> এই সত্যটা লুকানো।

---

## 6.2 Process, Task, Thread: Mental Clarity

### Process
- own address space
- own resources
- heavy isolation

### Thread
- shared address space
- separate execution state
- lighter

### Task (Kernel mental model)
> “Something that can be scheduled”

Kernel অনেক সময় process/thread আলাদা করে না।
সে দেখে:
- registers
- stack
- instruction pointer

---

## 6.3 Context মানে কী?

Context = CPU এর স্মৃতি

Includes:
- instruction pointer
- stack pointer
- registers
- flags

Context মানে:
> “আমি কে ছিলাম, কোথায় ছিলাম”

---

## 6.4 Context Switch: The Sleight of Hand

Context switch মানে:

```

save current context
load next context
jump

```

CPU নিজে এটা করে না।
Kernel করে।

Magic moment:
> CPU ভাবে কিছুই হয়নি।

---

## 6.5 Scheduling Trigger কোথা থেকে আসে?

Scheduling শুরু হয়:
- timer interrupt
- blocking operation
- explicit yield

Timer সবচেয়ে common।

Without timer:
> CPU never gives up control.

---

## 6.6 Preemptive vs Cooperative Scheduling

### Cooperative
- task নিজে yield করে
- polite world
- buggy code stalls system

### Preemptive
- timer forcibly interrupts
- fairness
- complexity

Modern OS = preemptive.

---

## 6.7 Run Queue: Waiting Line

Scheduler রাখে:
> runnable tasks এর list

Mental picture:

```

[ Task A ] -> [ Task B ] -> [ Task C ]

```

Scheduling মানে:
> কে পরের turn পাবে

---

## 6.8 Scheduling Policy = Behavior

Policy examples:
- round robin
- priority based
- real-time

Policy ঠিক করে:
- responsiveness
- fairness
- starvation

Engineering lesson:
> Scheduler policy defines system personality.

---

## 6.9 Timer Tick: The Metronome

Every tick:
- current task interrupted
- accounting done
- maybe switch

No tick:
> one task rules forever

Tick too frequent:
- overhead

Tick too slow:
- laggy system

Balance is art.

---

## 6.10 Stack Switching: The Hidden Danger

Each task has:
- its own stack

Context switch must:
- switch stack
- not corrupt previous one

Bug here:
> instant kernel panic

Kernel rule:
> Never trust the current stack during a switch.

---

## 6.11 Scheduler + Memory = Chaos Zone

During switch:
- heap mid-update?
- lock held?
- interrupt enabled?

Kernel protects:
- critical sections
- atomic ops
- interrupt disable

Mental model:
> scheduling multiplies all bugs

---

## 6.12 Why Scheduling Bugs Are Nightmare

Symptoms:
- random freeze
- works with debug print
- race-dependent

Cause:
> context switch at wrong time

Classic Heisenbugs.

---

## 6.13 Engineering Takeaway

> Scheduling is not about speed.
> It’s about fairness under chaos.

Simple scheduler that works
beats smart scheduler that breaks.

---

## 6.14 Chapter 6 Checkpoint

এই chapter শেষে তুমি পারবে:

- [ ] multitasking illusion explain করতে
- [ ] context switch mentally simulate করতে
- [ ] scheduling trigger list করতে
- [ ] preemptive vs cooperative তুলনা করতে
- [ ] scheduling + memory danger বুঝতে

---

## Final Thought

Kernel does not run programs.

Kernel **pauses reality**, swaps masks,
and resumes like nothing happened.
```

---

### এখান থেকে natural next step 🔜

Flow অনুযায়ী দুটো খুব strong option:

1️⃣ **Chapter 7: Synchronization (Locks, Races, Atomicity) – Mental-first**
→ scheduling এর natural consequence

2️⃣ **Chapter 6 Exercises (mental simulations)**
→ চিন্তা পোক্ত করার জন্য

আমার recommendation:

> আগে **Synchronization**, তারপর exercises।

বল, Chapter 7 খুলবো?
