একদম ঠিক জায়গায় এসে দাঁড়িয়েছো 👌
এখানেই MiniOS “kernel demo” থেকে **actual operating system** হয়ে ওঠে।

নিচেরটা তুমি সরাসরি docs এ রাখতে পারো।

👉 **`docs/chapter-8-system-calls-user-kernel-boundary.md`**

---

```markdown
# Chapter 8: System Calls (User ↔ Kernel Boundary)
## The Most Important Line a Program Is Not Allowed to Cross

> Goal:
> বুঝে ফেলা কেন user code সরাসরি kernel কে ছুঁতে পারে না,
> আর কিভাবে OS সেই ছোঁয়াটাকে controlled করে।

এই chapter শেষে তুমি বুঝবে:
> An OS is defined by its boundaries, not its features.

---

## 8.1 The Big Rule: Trust Nothing

User programs:
- crash
- lie
- cheat
- make mistakes

Kernel rule:
> Never trust user input. Ever.

System call exists because:
> kernel must stay in control.

---

## 8.2 User Mode vs Kernel Mode (Mental Model)

CPU has privilege levels.

Simplified:
- User mode: restricted
- Kernel mode: all power

User mode cannot:
- access hardware
- touch kernel memory
- disable interrupts

Crossing boundary requires permission.

---

## 8.3 What Is a System Call?

System call = **controlled entry point** into kernel.

Mental picture:

```

User Code
|
| syscall instruction
v
Kernel Entry

```

It is not a function call.
It is a **mode switch**.

---

## 8.4 Why Can’t We Just Call Kernel Functions?

Because:
- user stack is untrusted
- pointers may be invalid
- code may be malicious

Kernel must:
- validate everything
- copy carefully
- fail safely

Rule:
> All data from user is hostile.

---

## 8.5 System Call Lifecycle (Step-by-Step)

1. User prepares arguments
2. Executes syscall instruction
3. CPU switches to kernel mode
4. Kernel saves user context
5. Kernel validates arguments
6. Kernel performs operation
7. Kernel returns result
8. CPU switches back to user mode

This dance is sacred.

---

## 8.6 The Kernel Entry Gate

Entry gate responsibilities:
- save registers
- switch stack
- disable unsafe things
- identify syscall number

Bug here:
> kernel compromised instantly

---

## 8.7 System Call Table: Controlled Power

Kernel maintains:
- syscall number → function mapping

Mental model:
> whitelist, not blacklist

Unknown syscall:
> immediate reject

---

## 8.8 Copying Data Across the Boundary

Never trust user pointers.

Kernel uses:
- copy_from_user
- copy_to_user

Why?
> user memory may disappear mid-call

Direct dereference = fatal bug.

---

## 8.9 Blocking, Sleeping, and System Calls

Some syscalls:
- read
- wait
- sleep

These may:
- block
- trigger scheduling
- switch tasks

Syscall + scheduling = complexity.

---

## 8.10 System Calls and Interrupts

Syscalls are:
- synchronous
- initiated by user

Interrupts are:
- asynchronous
- initiated by hardware

Kernel treats both carefully,
but differently.

---

## 8.11 Error Handling: Honesty Over Comfort

Kernel must:
- return clear errors
- never crash
- never lie

User sees:
- error code
- partial success
- retry signals

Engineering rule:
> Loud failure beats silent corruption.

---

## 8.12 Security Begins Here

System calls are:
- attack surface
- security boundary
- policy enforcement point

One bad syscall check =
> full system compromise

---

## 8.13 Engineering Takeaway

> System calls are the constitution of the OS.

They define:
- what users can do
- how safely they can do it
- how much the kernel trusts them

---

## 8.14 Chapter 8 Checkpoint

এই chapter শেষে তুমি পারবে:

- [ ] user vs kernel mode explain করতে
- [ ] syscall কে function call থেকে আলাদা করতে
- [ ] boundary crossing mentally trace করতে
- [ ] pointer validation importance বুঝতে
- [ ] syscall + scheduling interaction reason করতে

---

## Final Thought

An operating system is not a program.

It is a **guardian**  
standing between chaos and hardware.
```

---

### এখন তুমি কোথায় দাঁড়িয়ে আছো 🧠

তুমি এখন বুঝেছো:

* memory is dangerous
* time is hostile
* concurrency is unavoidable
* kernel must not trust anyone

That’s real OS thinking.

---

### Natural next chapters 🔜

Flow অনুযায়ী আমার recommendation:

1️⃣ **Chapter 9: Files & IO (Abstractions Over Chaos)**
2️⃣ **Chapter 8 Exercises: Design a minimal syscall API**
3️⃣ **MiniOS Architecture Review (end-to-end)**

আমি personally বলবো:

> আগে **Files & IO**, তারপর review।

বল, Chapter 9 খুলবো?
