# MiniOS

MiniOS is a **minimal, educational purpose operating system** built to understand  
how real servers work **under the hood** — from memory and concurrency to networking and IO.

This project is **not** trying to replace Linux.  
It is designed to be **small, understandable, and brutally practical**.

---

## Status

🚧 Work in progress  
Built step by step as a learning project.

---

## Core Idea (Very Simple)

> Small systems teach big lessons.
>
> If you understand how an operating system manages memory, tasks, and IO, you will become a much stronger backend developer.


MiniOS focuses only on the parts that directly matter for backend systems. This OS is intentionally small so that **every line of code can be understood**.

<!-- ---

## What This OS Is NOT

- ❌ Not Linux
- ❌ Not POSIX compliant
- ❌ Not production-ready
- ❌ Not feature-complete

This is a **learning-first system**. -->

---

## Learning Goals

- How memory allocation really works
- How concurrency is scheduled
- How IO blocks and unblocks systems
- Why slow clients hurt performance
- How backend servers behave under load

---

## Roadmap

### Phase 1: Boot & Kernel Skeleton
- Minimal boot
- Kernel entry point
- Basic loop

**Takeaway:**  
Application startup and initialization order.

---

### Phase 2: Memory Management
- Stack vs heap
- Simple allocator
- Memory limits

**Takeaway:**  
Understanding memory usage, leaks, and GC pressure.

---

### Phase 3: Task Scheduling
- Cooperative tasks
- Simple scheduler
- Shared state issues

**Takeaway:**  
Worker pools, race conditions, and deadlocks.

---

### Phase 4: Syscall-like Interface
- Controlled entry points
- Argument validation
- Error handling

**Takeaway:**  
API design, trust boundaries, and validation.

---

### Phase 5: Networking Basics
- Socket abstraction
- Blocking vs non-blocking IO
- Buffers

**Takeaway:**  
HTTP behavior, timeouts, slow clients.

---

### Phase 6: File & Persistence
- Read/write abstraction
- Simple storage
- Durability concepts

**Takeaway:**  
Database latency, logging, and storage tradeoffs.

---

### Phase 7: Stability & Shutdown
- Graceful shutdown
- Resource cleanup
- Error boundaries

**Takeaway:**
Production readiness and resilience.

---

## Target Audience

- Backend developers curious about systems
- Engineers interested in performance and reliability

---

## License

MIT
