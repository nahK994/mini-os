200,000 concurrent connections
```

তখন **thread-per-connection model** কেন ভেঙে পড়ে?

কিন্তু event-driven architecture (যেমন
NGINX) কেন ঠিক থাকে?

এটা বুঝলে তুমি OS + backend architecture এর খুব গুরুত্বপূর্ণ জায়গায় পৌঁছে যাবে।

---

চল এবার যুক্তিটা ধীরে খুলে দেখি। এখানে অনেক backend engineer প্রথমে একটা ভুল ধারণা ধরে নেয়, তাই আগে সেটাকে পরীক্ষা করি। 🧠

---

# প্রথমে একটি অনুমান পরীক্ষা করি

অনেকে ভাবে:

> “যত বেশি connection, তত বেশি thread বানালেই হবে।”

এটাই **thread-per-connection model**।

মানে:

```
1 connection → 1 thread
```

এখন তোমার প্রশ্নের scenario:

```
200,000 connections
```

তাহলে দরকার:

```
200,000 threads
```

এখানেই সিস্টেম ভেঙে পড়ে। কেন? চল একে একে দেখি।

---

# ১️⃣ Thread memory explosion

প্রতিটা thread এর একটি **stack memory** লাগে।

Linux-এ default thread stack সাধারণত প্রায়:

```
8 MB
```

এখন হিসাব করো:

```
200,000 × 8MB
```

≈ **1.6 TB RAM**

এটা বাস্তব server-এ অসম্ভব।

এখানেই thread-per-connection model প্রথম ধাক্কা খায়।

---

# ২️⃣ Context switching catastrophe

CPU একসাথে সব thread চালায় না।

এটা একটার পর একটা চালায়।

যখন CPU এক thread থেকে আরেকটায় যায় তাকে বলে:

```
context switch
```

যদি 200k thread থাকে:

CPU বারবার thread switch করবে।

Result:

* cache miss
* scheduler overhead
* latency spike

CPU কাজের চেয়ে thread বদলাতেই সময় নষ্ট করবে।

---

# ৩️⃣ Scheduler collapse

Linux scheduler-কে 200k thread manage করতে হবে।

Scheduler queue বিশাল হয়ে যায়।

Result:

* unfair scheduling
* latency unpredictability

Server unstable হয়ে যায়।

---

# তাহলে event-driven architecture কী করে?

এখানেই magic।

ধরো server architecture:

* NGINX
* Node.js

এরা **thread-per-connection ব্যবহার করে না**।

এরা ব্যবহার করে:

```
event loop
```

---

# Event-driven model

এখানে architecture:

```
1 thread
(or few threads)

→ handles thousands of connections
```

কিভাবে?

OS feature ব্যবহার করে:

```
epoll
```

এটা kernel-কে বলে:

> "যখন কোন socket এ data আসবে তখন আমাকে জানাও।"

অর্থাৎ:

CPU idle বসে থাকে না।

Event এ কাজ করে।

---

# Example flow

ধরো 50,000 socket open।

event loop এভাবে কাজ করে:

```
while true:
    events = epoll_wait()
    handle(events)
```

CPU শুধু active connection handle করে।

Idle connection প্রায় zero cost।

---

# Memory comparison

Thread model:

```
200k threads
≈ TBs RAM
```

Event loop model:

```
200k sockets
≈ few hundred MB
```

এটাই huge difference।

---

# বাস্তব পৃথিবীর server design

এই কারণেই modern high-performance server:

* NGINX
* Redis
* HAProxy

সবাই **event-driven architecture** ব্যবহার করে।

---

# কিন্তু একটা subtle point আছে

এখানে একটা ভুল ধারণা ভাঙা দরকার।

অনেকে ভাবে:

> “event loop সবসময় faster”

সবসময় না।

যদি workload হয়:

* heavy CPU computation
* video encoding
* ML inference

তাহলে thread pool model ভালো হতে পারে।

Event loop best হয় যখন:

```
IO heavy
CPU light
```

যেমন:

* web server
* proxy
* chat server
* websocket

---

# এখন একটা deeper systems question

ধরো server:

```
100k idle websocket connection
```

CPU usage প্রায় **0%**।

কিন্তু suddenly সব client message পাঠালো।

তখন system-এ প্রথম pressure কোথায় পড়বে?

1️⃣ CPU
2️⃣ network buffer
3️⃣ event loop queue
4️⃣ memory allocator

যদি এটা explain করতে পারো, তাহলে তুমি সত্যি **backend + OS boundary** বুঝতে শুরু করেছ।
