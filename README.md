# MiniOS

MiniOS is a **minimal, educational operating system** built to deeply understand  
how real systems work **under the hood** — from memory and execution to isolation and IO.

This project is **not** about replacing Linux.  
It is about **building engineering intuition** by stripping systems down to their core.

---

## Status

🚧 Work in progress

---

## Core Philosophy

Small systems teach big lessons.

---

## Engineering Roadmap (Chapters)

### Chapter 0: Engineering Grounding (Mental Model)
**Goal:** Think like a systems engineer before writing code.

**Concepts**
- Program vs Process vs Kernel
- Memory as a byte array
- Pointer vs ownership
- Undefined behavior and why kernels fear it

**Checkpoint**
- Able to explain why dangling pointers are dangerous
- Able to reason about memory without tools

---

### Chapter 1: Boot & Kernel Skeleton
**Goal:** Understand how code starts running on bare metal.

**Concepts**
- CPU reset state
- Bootloader responsibility
- Real mode vs Protected mode
- Jumping into the kernel

**Engineering Takeaway**
> Initialization order matters more than code elegance.

---

### Chapter 2: Memory Is the First Lie
**Goal:** Learn how memory really works inside a kernel.

**Concepts**
- Linker script and symbols (`_kernel_end`)
- Stack vs heap (OS perspective)
- Alignment and invariants
- Bump allocator
- Why early `kfree()` is fake but useful

**Engineering Takeaway**
> Allocation strategy defines system behavior.

**Checkpoint**
- Able to explain why `kfree()` doesn’t delete variables
- Able to explain allocator invariants
- Able to debug memory corruption mentally

---

### Chapter 3: Memory Stops Being Linear
**Goal:** Understand why virtual memory exists.

**Concepts**
- Paging
- Page tables
- MMU
- Address translation
- Isolation

**Engineering Takeaway**
> Memory is not what you touch, it’s what the CPU allows you to see.

---

### Chapter 4: Time Is a Shared Resource
**Goal:** Learn how execution is shared.

**Concepts**
- Interrupts
- Timers
- Context switching
- Scheduling strategies

**Engineering Takeaway**
> CPU time is more valuable than memory.

---

### Chapter 5: Trust Boundaries & Syscalls
**Goal:** Separate user intent from kernel authority.

**Concepts**
- Privilege levels
- Syscall interface
- Validation
- Fault handling

**Engineering Takeaway**
> User code is untrusted by default.

---

### Chapter 6: IO, Files, and Persistence
**Goal:** Understand durability and latency.

**Concepts**
- File abstractions
- Buffers
- Blocking vs non-blocking IO
- Persistence guarantees

**Engineering Takeaway**
> Fast systems are designed, not optimized.

---

### Chapter 7: Stability, Failure, and Shutdown
**Goal:** Build systems that fail predictably.

**Concepts**
- Resource cleanup
- Error boundaries
- Graceful shutdown
- Panic paths

**Engineering Takeaway**
> Production systems are defined by how they fail.

---

## Target Audience

- Engineers curious about performance, memory, and reliability
- Anyone who wants to **understand before scaling**

---

## Learning Contract

This project follows a strict rule:

> No phase skipping.  
> No magical code.  
> No “just trust this”.

Every concept must be explainable in plain language.

---

## License

MIT
