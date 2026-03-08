ধরো তোমার server এ **৫০,০০০ concurrent connection** এসেছে।

তখন কোনটা bottleneck হওয়ার সম্ভাবনা বেশি?

1️⃣ CPU
2️⃣ Memory
3️⃣ File descriptors
4️⃣ Network backlog

সঠিক উত্তরটা কেন হবে সেটাও explain করতে পারলে বুঝবো তুমি OS layer ধরতে শুরু করেছ।

---

৫০,০০০ concurrent connection হলে চারটা জিনিসই bottleneck হতে পারে। কিন্তু বাস্তবে **সবচেয়ে আগে যেটা প্রায়ই ধাক্কা খায় সেটা হলো:**

**➡️ File descriptors**

এটা অনেক backend engineer production-এ গিয়ে প্রথমবার বুঝে। সার্ভার যেন দরজাওয়ালা এক বিশাল ভবন। প্রতিটা connection মানে একটা দরজা। OS প্রতিটা দরজাকে একটি **file descriptor** দিয়ে চিহ্নিত করে। 🚪

---

# ১️⃣ File Descriptor কেন প্রথম bottleneck হয়

Linux-এ প্রতিটা process এর জন্য open file descriptor limit থাকে।

ধরো:

```
ulimit -n
```

অনেক server-এ default value হয়:

```
1024
```

মানে একটি process সর্বোচ্চ ১০২৪টা file descriptor খুলতে পারবে।

এখন মনে রাখো Linux-এ:

* socket = file descriptor
* file = file descriptor
* pipe = file descriptor

অর্থাৎ:

**প্রতিটা TCP connection = ১টা file descriptor**

---

### এখন হিসাবটা দেখো

ধরো server-এ এসেছে:

```
50,000 connections
```

তখন server-কে দরকার:

```
50,000 file descriptors
```

কিন্তু limit যদি হয়:

```
1024
```

তাহলে OS বলবে:

```
Too many open files
```

Server নতুন connection accept করতে পারবে না।

---

# ২️⃣ Network backlog কেন দ্বিতীয় সম্ভাব্য bottleneck

TCP connection আসলে কয়েকটা ধাপে তৈরি হয়:

1. SYN
2. SYN-ACK
3. ACK

Server-এর listen queue থাকে।

যদি queue ছোট হয়:

```
net.core.somaxconn
```

তাহলে incoming connection drop হতে পারে।

---

# ৩️⃣ Memory কখন bottleneck হয়

প্রতিটা connection কিছু memory ব্যবহার করে।

Example:

* socket buffer
* request buffer
* goroutine / thread stack

যদি প্রতিটা connection 50KB নেয়:

```
50,000 × 50KB
≈ 2.5GB RAM
```

তখন RAM bottleneck হতে পারে।

কিন্তু অনেক server memory optimize করে।

উদাহরণ:

* NGINX
* Redis

এরা event loop architecture ব্যবহার করে।

---

# ৪️⃣ CPU সাধারণত পরে bottleneck হয়

Connection idle থাকলে CPU ব্যবহার খুব কম।

Example:

50k idle websocket connection।

CPU প্রায় কিছুই করছে না।

CPU bottleneck হয় যখন:

* heavy computation
* encryption
* serialization

---

# বাস্তব production order (সাধারণত)

High concurrency server-এ bottleneck হওয়ার সম্ভাবনা:

```
1️⃣ File descriptor limit
2️⃣ Network backlog
3️⃣ Memory
4️⃣ CPU
```

---

# একটা বাস্তব example

ধরো তুমি Go দিয়ে server বানালে।

50k connection এ error:

```
accept: too many open files
```

সমাধান:

```
ulimit -n 100000
```

তারপর system level limit বাড়াতে হয়।

---

# এখন তোমার জন্য একটা deeper প্রশ্ন 🧠

ধরো server এ:

```
200,000 concurrent connections
```

তখন **thread-per-connection model** কেন ভেঙে পড়ে?

কিন্তু event-driven architecture (যেমন
NGINX) কেন ঠিক থাকে?

এটা বুঝলে তুমি OS + backend architecture এর খুব গুরুত্বপূর্ণ জায়গায় পৌঁছে যাবে।
