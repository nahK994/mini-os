# MiniOS (working name)

MiniOS is a **minimal, educational operating system** built to help backend developers understand  
how real servers work **under the hood** — from memory and concurrency to networking and IO.

This project is **not** trying to replace Linux.  
It is designed to be **small, understandable, and brutally practical**.

---

## Why this project exists

As backend developers, we build APIs, services, caches, and databases every day.

But many important questions stay unclear:

- Why does my server slow down under load?
- Why do goroutines block or leak?
- Why is memory usage unpredictable?
- Why does disk IO or networking become a bottleneck?

MiniOS exists to answer these questions by **building the fundamentals ourselves**.

---

## Core Idea (Very Simple)

> If you understand how an operating system manages memory, tasks, and IO,  
> you will become a much stronger backend developer.

MiniOS focuses only on the parts that directly matter for backend systems.

---

## Selling Points

### 1. Built for Backend Developers
This OS is not academic and not POSIX-complete.

Every concept maps directly to backend problems:
- Tasks ≈ Requests
- Scheduler ≈ Worker pool
- Syscalls ≈ API boundaries
- Memory pressure ≈ GC pressure

---

### 2. Minimal Scope, Maximum Clarity
- No complex drivers
- No legacy compatibility
- No unnecessary abstractions

Only:
- Memory management
- Task scheduling
- Networking basics
- File and IO handling

---

### 3. Request-Centric Design
Instead of focusing on full processes, MiniOS treats **requests** as first-class citizens.

This makes it easier to reason about:
- Timeouts
- Fairness
- Backpressure
- Resource limits

---

### 4. Failure Is a Feature
The system is intentionally designed to:
- Handle slow clients
- Handle partial writes
- Expose crashes clearly
- Teach graceful shutdown

Just like real production systems.

---

### 5. Observability First
The OS exposes:
- Task counts
- Memory usage
- Queue depth
- Simple logs

Because production systems live and die by observability.

---

## What This OS Is NOT

- ❌ Not Linux
- ❌ Not POSIX compliant
- ❌ Not production-ready
- ❌ Not feature-complete

This is a **learning-first system**.

---

## Learning Goals

By building MiniOS, you will learn:

- How memory allocation really works
- How concurrency is scheduled
- How IO blocks and unblocks systems
- Why slow clients hurt performance
- How backend servers behave under load

---

## Roadmap (Backend-Focused)

### Phase 1: Boot & Kernel Skeleton
- Minimal boot
- Kernel entry point
- Basic loop

**Backend takeaway:**  
Application startup and initialization order.

---

### Phase 2: Memory Management
- Stack vs heap
- Simple allocator
- Memory limits

**Backend takeaway:**  
Understanding memory usage, leaks, and GC pressure.

---

### Phase 3: Task Scheduling
- Cooperative tasks
- Simple scheduler
- Shared state issues

**Backend takeaway:**  
Worker pools, race conditions, and deadlocks.

---

### Phase 4: Syscall-like Interface
- Controlled entry points
- Argument validation
- Error handling

**Backend takeaway:**  
API design, trust boundaries, and validation.

---

### Phase 5: Networking Basics
- Socket abstraction
- Blocking vs non-blocking IO
- Buffers

**Backend takeaway:**  
HTTP behavior, timeouts, slow clients.

---

### Phase 6: File & Persistence
- Read/write abstraction
- Simple storage
- Durability concepts

**Backend takeaway:**  
Database latency, logging, and storage tradeoffs.

---

### Phase 7: Stability & Shutdown
- Graceful shutdown
- Resource cleanup
- Error boundaries

**Backend takeaway:**  
Production readiness and resilience.

---

## Target Audience

- Backend developers curious about systems
- Developers who want to go beyond CRUD
- Engineers interested in performance and reliability
- Anyone who wants to understand servers deeply

---

## Philosophy

> Small systems teach big lessons.

This OS is intentionally small so that **every line of code can be understood**.

---

## Status

🚧 Work in progress  
Built step by step as a learning project.

---

## License

MIT
