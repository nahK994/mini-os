## Chapter 4

**অধ্যায় ৪**

## Output

**আউটপুট**

---

This chapter will present how to display text on the console as well as writing data to the serial port.
এই অধ্যায়ে কনসোলে কীভাবে টেক্সট প্রদর্শন করতে হয় এবং সিরিয়াল পোর্টে কীভাবে ডাটা লেখা যায় তা দেখানো হবে।

Furthermore, we will create our first driver, that is, code that acts as a layer between the kernel and the hardware, providing a higher abstraction than communicating directly with the hardware.
এছাড়াও আমরা আমাদের প্রথম ড্রাইভার তৈরি করব, অর্থাৎ এমন কোড যা kernel এবং hardware-এর মাঝখানে একটি স্তর হিসেবে কাজ করে এবং সরাসরি hardware-এর সাথে যোগাযোগের চেয়ে উচ্চতর abstraction প্রদান করে।

The first part of this chapter is about creating a driver for the framebuffer [26] to be able to display text on the console.
এই অধ্যায়ের প্রথম অংশে framebuffer-এর জন্য একটি ড্রাইভার তৈরি করা হবে যাতে কনসোলে টেক্সট প্রদর্শন করা যায়।

The second part shows how to create a driver for the serial port.
দ্বিতীয় অংশে সিরিয়াল পোর্টের জন্য কীভাবে ড্রাইভার তৈরি করতে হয় তা দেখানো হবে।

Bochs can store output from the serial port in a file, effectively creating a logging mechanism for the operating system.
Bochs সিরিয়াল পোর্টের আউটপুট একটি ফাইলে সংরক্ষণ করতে পারে, ফলে এটি অপারেটিং সিস্টেমের জন্য একটি লগিং মেকানিজম তৈরি করে।

---

## Interacting with the Hardware

**হার্ডওয়্যারের সাথে যোগাযোগ**

There are usually two different ways to interact with the hardware, memory-mapped I/O and I/O ports.
সাধারণত হার্ডওয়্যারের সাথে যোগাযোগ করার দুটি উপায় আছে: memory-mapped I/O এবং I/O ports।

---

If the hardware uses memory-mapped I/O then you can write to a specific memory address and the hardware will be updated with the new data.
যদি হার্ডওয়্যার memory-mapped I/O ব্যবহার করে, তবে নির্দিষ্ট একটি মেমরি অ্যাড্রেসে লিখলেই হার্ডওয়্যার নতুন ডাটা দিয়ে আপডেট হবে।

One example of this is the framebuffer, which will be discussed in more detail later.
এর একটি উদাহরণ হলো framebuffer, যা পরে বিস্তারিত আলোচনা করা হবে।

For example, if you write the value 0x410F to address 0x000B8000, you will see the letter A in white color on a black background (see the section on the framebuffer for more details).
উদাহরণস্বরূপ, যদি তুমি 0x000B8000 অ্যাড্রেসে 0x410F লিখো, তাহলে কালো ব্যাকগ্রাউন্ডে সাদা রঙের A অক্ষর দেখতে পাবে (আরও বিস্তারিত framebuffer অংশে দেখো)।

---

If the hardware uses I/O ports then the assembly code instructions out and in must be used to communicate with the hardware.
যদি হার্ডওয়্যার I/O ports ব্যবহার করে, তবে হার্ডওয়্যারের সাথে যোগাযোগ করার জন্য assembly instruction `out` এবং `in` ব্যবহার করতে হবে।

The instruction out takes two parameters: the address of the I/O port and the data to send.
`out` instruction দুটি প্যারামিটার নেয়: I/O port-এর ঠিকানা এবং পাঠানোর ডাটা।

The instruction in takes a single parameter, the address of the I/O port, and returns data from the hardware.
`in` instruction একটি প্যারামিটার নেয়, যা হলো I/O port-এর ঠিকানা, এবং হার্ডওয়্যার থেকে ডাটা ফেরত দেয়।

One can think of I/O ports as communicating with hardware the same way as you communicate with a server using sockets.
I/O ports-কে এমনভাবে ভাবা যেতে পারে যেন socket ব্যবহার করে সার্ভারের সাথে যোগাযোগ করা হচ্ছে।

The cursor (the blinking rectangle) of the framebuffer is one example of hardware controlled via I/O ports on a PC.
Framebuffer-এর cursor (ঝিলমিল করা আয়তাকার চিহ্ন) হলো এমন একটি উদাহরণ যা PC-তে I/O ports দিয়ে নিয়ন্ত্রিত হয়।

---

## The Framebuffer

**ফ্রেমবাফার**

The framebuffer is a hardware device that is capable of displaying a buffer of memory on the screen [26].
Framebuffer হলো একটি হার্ডওয়্যার ডিভাইস যা মেমরির একটি buffer স্ক্রিনে প্রদর্শন করতে পারে।

The framebuffer has 80 columns and 25 rows, and the row and column indices start at 0 (so rows are labelled 0 - 24).
Framebuffer-এ ৮০টি কলাম এবং ২৫টি সারি আছে, এবং row ও column-এর সূচক ০ থেকে শুরু হয় (অর্থাৎ row গুলো ০ থেকে ২৪ পর্যন্ত)।

---

## Writing Text

**টেক্সট লেখা**

Writing text to the console via the framebuffer is done with memory-mapped I/O.
Framebuffer ব্যবহার করে কনসোলে টেক্সট লেখা হয় memory-mapped I/O দিয়ে।

The starting address of the memory-mapped I/O for the framebuffer is 0x000B8000 [27].
Framebuffer-এর জন্য memory-mapped I/O-এর শুরুর ঠিকানা হলো 0x000B8000।

The memory is divided into 16 bit cells,
এই মেমরি ১৬-বিটের সেলে ভাগ করা থাকে,

where the 16 bits determine both the character, the foreground color and the background color.
যেখানে এই ১৬টি বিট একসাথে character, foreground color এবং background color নির্ধারণ করে।

The highest eight bits is the ASCII [28] value of the character, bit 7 - 4 the background and bit 3 - 0 the foreground, as can be seen in the following figure:
সর্বোচ্চ ৮ বিট হলো character-এর ASCII মান, বিট ৭-৪ হলো background color এবং বিট ৩-০ হলো foreground color, যা নিচের চিত্রে দেখানো হয়েছে:

```
Bit:     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
Content: | ASCII                | BG      | FG      |
```

---

The available colors are shown in the following table:
উপলব্ধ রঙগুলো নিচের টেবিলে দেখানো হয়েছে:

Color Value Color Value Color Value Color Value
রঙ মান | রঙ মান | রঙ মান | রঙ মান

Black 0
কালো ০

Red 4
লাল ৪

Dark grey 8
গাঢ় ধূসর ৮

Light red 12
হালকা লাল ১২

Blue 1
নীল ১

Magenta 5
ম্যাজেন্টা ৫

Light blue 9
হালকা নীল ৯

Light magenta 13
হালকা ম্যাজেন্টা ১৩

Green 2
সবুজ ২

Brown 6
বাদামি ৬

Light green 10
হালকা সবুজ ১০

Light brown 14
হালকা বাদামি ১৪

Cyan 3
সায়ান ৩

Light grey 7
হালকা ধূসর ৭

Light cyan 11
হালকা সায়ান ১১

White 15
সাদা ১৫

---

The first cell corresponds to row zero, column zero on the console.
প্রথম cell কনসোলের row zero এবং column zero-এর সাথে সম্পর্কিত।

Using an ASCII table, one can see that A corresponds to 65 or 0x41.
ASCII টেবিল ব্যবহার করলে দেখা যায় যে A-এর মান হলো 65 অথবা 0x41।

Therefore, to write the character A with a green foreground (2) and dark grey background (8) at place (0,0), the following assembly code instruction is used:
সুতরাং, (0,0) অবস্থানে সবুজ foreground (2) এবং গাঢ় ধূসর background (8) সহ A অক্ষর লেখার জন্য নিচের assembly instruction ব্যবহার করা হয়:

```asm
mov [0x000B8000], 0x4128
```

The second cell then corresponds to row zero, column one and its address is therefore:
দ্বিতীয় cell তখন row zero, column one-এর সাথে সম্পর্কিত হবে এবং এর ঠিকানা হবে:

```text
0x000B8000 + 16 = 0x000B8010
```

---

Writing to the framebuffer can also be done in C by treating the address 0x000B8000 as a char pointer,
Framebuffer-এ লেখা C ভাষাতেও করা যায়, যদি 0x000B8000 ঠিকানাটিকে char pointer হিসেবে ধরা হয়,

```c
char *fb = (char *) 0x000B8000.
```

Then, writing A at place (0,0) with green foreground and dark grey background becomes:
তখন (0,0) অবস্থানে সবুজ foreground এবং গাঢ় ধূসর background সহ A লেখার কোড হবে:

```c
fb[0] = 'A';
fb[1] = 0x28;
```

---

The following code shows how this can be wrapped into a function:
নিচের কোডটি দেখায় কীভাবে এটিকে একটি function-এর মধ্যে রাখা যায়:

```c
/** fb_write_cell:
* Writes a character with the given foreground and background to position i
* in the framebuffer.
*
* @param i The location in the framebuffer
* @param c The character
* @param fg The foreground color
* @param bg The background color
*/

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{

fb[i] = c;
fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F)

}
```

---

The function can then be used as follows:
এই function-টি নিচের মতো করে ব্যবহার করা যায়:

```c
#define FB_GREEN 2
#define FB_DARK_GREY 8

fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
```

---

## Moving the Cursor

**কার্সর সরানো**

Moving the cursor of the framebuffer is done via two different I/O ports.
Framebuffer-এর cursor সরানো হয় দুটি ভিন্ন I/O port ব্যবহার করে।

The cursor’s position is determined with a 16 bits integer:
Cursor-এর অবস্থান একটি ১৬-বিট integer দিয়ে নির্ধারণ করা হয়:

0 means row zero, column zero;
0 মানে row zero, column zero;

1 means row zero, column one;
1 মানে row zero, column one;

80 means row one, column zero and so on.
80 মানে row one, column zero, এভাবে চলতে থাকে।

Since the position is 16 bits large, and the out assembly code instruction argument is 8 bits, the position must be sent in two turns, first 8 bits then the next 8 bits.
যেহেতু position ১৬ বিটের এবং `out` assembly instruction একবারে ৮ বিট নিতে পারে, তাই position দুই ধাপে পাঠাতে হবে—প্রথমে উচ্চ ৮ বিট, পরে নিচের ৮ বিট।

The framebuffer has two I/O ports, one for accepting the data, and one for describing the data being received.
Framebuffer-এর দুটি I/O port আছে, একটি ডাটা গ্রহণ করার জন্য এবং আরেকটি কোন ডাটা আসছে তা বোঝানোর জন্য।

Port 0x3D4 [29] is the port that describes the data and port 0x3D5 [29] is for the data itself.
0x3D4 পোর্টটি ডাটা বর্ণনার জন্য এবং 0x3D5 পোর্টটি আসল ডাটার জন্য ব্যবহৃত হয়।

---

To set the cursor at row one, column zero (position 80 = 0x0050), one would use the following assembly code instructions:
Cursor-কে row one, column zero-তে (position 80 = 0x0050) সেট করতে নিচের assembly instruction ব্যবহার করা হয়:

```asm
out 0x3D4, 14 ; 14 tells the framebuffer to expect the highest 8 bits of the position
out 0x3D5, 0x00 ; sending the highest 8 bits of 0x0050
out 0x3D4, 15 ; 15 tells the framebuffer to expect the lowest 8 bits of the position
out 0x3D5, 0x50 ; sending the lowest 8 bits of 0x0050
```

14 tells the framebuffer to expect the highest 8 bits of the position
14 framebuffer-কে বলে যে এখন position-এর উচ্চ ৮ বিট পাঠানো হবে

15 tells the framebuffer to expect the lowest 8 bits of the position
15 framebuffer-কে বলে যে এখন position-এর নিচের ৮ বিট পাঠানো হবে

---

The out assembly code instruction can’t be executed directly in C.
`out` assembly instruction সরাসরি C থেকে চালানো যায় না।

Therefore it is a good idea to wrap out in a function in assembly code which can be accessed from C via the cdecl calling standard [25]:
তাই `out` instruction-কে assembly function আকারে লেখা ভালো, যেটি C থেকে cdecl calling standard ব্যবহার করে ডাকা যাবে:

```asm
global outb ; make the label outb visible outside this file

; outb - send a byte to an I/O port
; stack: [esp + 8] the data byte
; [esp + 4] the I/O port
; [esp ] return address
outb:

mov al, [esp + 8] ; move the data to be sent into the al register
mov dx, [esp + 4] ; move the address of the I/O port into the dx register
out dx, al ; send the data to the I/O port
ret ; return to the calling function
```


ঠিক আছে 👍
এখন দিচ্ছি **Part 3** — আগের জায়গা থেকে আবার **লাইন বাই লাইন (English → Bangla)** অনুবাদ, কোনো লাইন বাদ না দিয়ে, Markdown ফরম্যাটে।

---

By storing this function in a file called io.s and also creating a header io.h, the out assembly code instruction can be conveniently accessed from C:
এই ফাংশনটি `io.s` নামে একটি ফাইলে রেখে এবং একটি `io.h` হেডার ফাইল তৈরি করলে, `out` assembly instruction সহজে C থেকে ব্যবহার করা যাবে:

```c id="7f3l2k"
#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/** outb:
* Sends the given data to the given I/O port. Defined in io.s
*
* @param port The I/O port to send the data to
* @param data The data to send to the I/O port
*/

void outb(unsigned short port, unsigned char data);

#endif /* INCLUDE_IO_H */
```

---

Moving the cursor can now be wrapped in a C function:
এখন cursor সরানোর কাজটি একটি C function-এর মধ্যে রাখা যেতে পারে:

```c id="0vm3k2"
#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/** fb_move_cursor:
* Moves the cursor of the framebuffer to the given position
*
* @param pos The new position of the cursor
*/

void fb_move_cursor(unsigned short pos)
{

outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
outb(FB_DATA_PORT, pos & 0x00FF);

}
```

---

## The Driver

**ড্রাইভার**

The driver should provide an interface that the rest of the code in the OS will use for interacting with the framebuffer.
ড্রাইভারটি এমন একটি ইন্টারফেস সরবরাহ করবে যা অপারেটিং সিস্টেমের বাকি কোড framebuffer-এর সাথে যোগাযোগ করতে ব্যবহার করবে।

There is no right or wrong in what functionality the interface should provide, but a suggestion is to have a write function with the following declaration:
এই ইন্টারফেসে কী ধরনের ফাংশন থাকবে তার কোনো নির্দিষ্ট সঠিক বা ভুল নেই, তবে একটি প্রস্তাব হলো নিচের মতো একটি write ফাংশন রাখা:

```c id="z3k1k4"
int write(char *buf, unsigned int len);
```

The write function writes the contents of the buffer buf of length len to the screen.
write ফাংশনটি buf নামের buffer-এর len দৈর্ঘ্যের কনটেন্ট স্ক্রিনে লিখে।

The write function should automatically advance the cursor after a character has been written and scroll the screen if necessary.
write ফাংশনটি প্রতিটি character লেখার পর স্বয়ংক্রিয়ভাবে cursor এগিয়ে নেবে এবং প্রয়োজন হলে স্ক্রিন স্ক্রল করবে।

---

## The Serial Ports

**সিরিয়াল পোর্টসমূহ**

The serial port [30] is an interface for communicating between hardware devices and although it is available on almost all motherboards, it is seldom exposed to the user in the form of a DE-9 connector nowadays.
সিরিয়াল পোর্ট হলো হার্ডওয়্যার ডিভাইসগুলোর মধ্যে যোগাযোগের একটি ইন্টারফেস, এবং যদিও এটি প্রায় সব মাদারবোর্ডে থাকে, বর্তমানে এটি খুব কম ক্ষেত্রেই ব্যবহারকারীর কাছে DE-9 কানেক্টর আকারে দেখা যায়।

The serial port is easy to use, and, more importantly, it can be used as a logging utility in Bochs.
সিরিয়াল পোর্ট ব্যবহার করা সহজ, এবং সবচেয়ে গুরুত্বপূর্ণ হলো এটি Bochs-এ একটি logging utility হিসেবে ব্যবহার করা যায়।

If a computer has support for a serial port, then it usually has support for multiple serial ports, but we will only make use of one of the ports.
যদি কোনো কম্পিউটার সিরিয়াল পোর্ট সমর্থন করে, তবে সাধারণত একাধিক সিরিয়াল পোর্ট থাকে, কিন্তু আমরা শুধু একটি পোর্ট ব্যবহার করব।

This is because we will only use the serial ports for logging.
এর কারণ হলো আমরা সিরিয়াল পোর্ট শুধু logging-এর জন্য ব্যবহার করব।

Furthermore, we will only use the serial ports for output, not input.
এছাড়াও, আমরা সিরিয়াল পোর্ট শুধুমাত্র output-এর জন্য ব্যবহার করব, input-এর জন্য নয়।

The serial ports are completely controlled via I/O ports.
সিরিয়াল পোর্ট সম্পূর্ণভাবে I/O ports দিয়ে নিয়ন্ত্রিত হয়।

---

## Configuring the Serial Port

**সিরিয়াল পোর্ট কনফিগার করা**

The first data that need to be sent to the serial port is configuration data.
সিরিয়াল পোর্টে প্রথমে যে ডাটা পাঠাতে হবে তা হলো configuration data।

In order for two hardware devices to be able to talk to each other they must agree upon a couple of things.
দুটি হার্ডওয়্যার ডিভাইস একে অপরের সাথে যোগাযোগ করতে পারার জন্য কিছু বিষয়ে একমত হতে হয়।

These things include:
এই বিষয়গুলোর মধ্যে রয়েছে:

• The speed used for sending data (bit or baud rate)
• ডাটা পাঠানোর গতি (bit বা baud rate)

• If any error checking should be used for the data (parity bit, stop bits)
• ডাটার জন্য কোনো error checking ব্যবহার করা হবে কিনা (parity bit, stop bit)

• The number of bits that represent a unit of data (data bits)
• একটি ডাটা ইউনিট কত বিট দিয়ে গঠিত হবে (data bits)

---

## Configuring the Line

**লাইন কনফিগার করা**

Configuring the line means to configure how data is being sent over the line.
লাইন কনফিগার করার মানে হলো ডাটা কীভাবে লাইনের মাধ্যমে পাঠানো হবে তা নির্ধারণ করা।

The serial port has an I/O port, the line command port, that is used for configuration.
সিরিয়াল পোর্টের একটি I/O port আছে যাকে line command port বলা হয়, যা কনফিগারেশনের জন্য ব্যবহৃত হয়।

First the speed for sending data will be set.
প্রথমে ডাটা পাঠানোর গতি নির্ধারণ করা হবে।

The serial port has an internal clock that runs at 115200 Hz.
সিরিয়াল পোর্টের একটি অভ্যন্তরীণ clock আছে যা 115200 Hz গতিতে চলে।

Setting the speed means sending a divisor to the serial port, for example sending 2 results in a speed of 115200 / 2 = 57600 Hz.
গতি নির্ধারণ করার মানে হলো সিরিয়াল পোর্টে একটি divisor পাঠানো; উদাহরণস্বরূপ 2 পাঠালে গতি হবে 115200 / 2 = 57600 Hz।

---

The divisor is a 16 bit number but we can only send 8 bits at a time.
divisor একটি ১৬-বিট সংখ্যা, কিন্তু আমরা একবারে মাত্র ৮ বিট পাঠাতে পারি।

We must therefore send an instruction telling the serial port to first expect the highest 8 bits, then the lowest 8 bits.
তাই আমাদের একটি instruction পাঠাতে হবে যা সিরিয়াল পোর্টকে জানাবে যে প্রথমে উচ্চ ৮ বিট পাঠানো হবে, তারপর নিচের ৮ বিট।

This is done by sending 0x80 to the line command port.
এটি করা হয় line command port-এ 0x80 পাঠিয়ে।

An example is shown below:
নিচে একটি উদাহরণ দেখানো হলো:

```c id="19m2dd"
#include "io.h" /* io.h is implement in the section "Moving the cursor" */

/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
* all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
* order, but they start at different values.
*/

#define SERIAL_COM1_BASE 0x3F8 /* COM1 base port */

#define SERIAL_DATA_PORT(base) (base)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
* Tells the serial port to expect first the highest 8 bits on the data port,
* then the lowest 8 bits will follow
*/

#define SERIAL_LINE_ENABLE_DLAB 0x80
```

ঠিক আছে। এখন দিচ্ছি **Part 5 (শুধু অনুবাদ অংশ)** — আগের জায়গা থেকে শেষ পর্যন্ত **লাইন বাই লাইন বাংলায় অনুবাদ**।

---

Name Description
নাম বর্ণনা

r Reserved
r = সংরক্ষিত (Reserved)

af Autoflow control enabled
af = Autoflow control চালু থাকে

lb Loopback mode (used for debugging serial ports)
lb = Loopback mode (সিরিয়াল পোর্ট ডিবাগ করার জন্য ব্যবহৃত)

ao2 Auxiliary output 2, used for receiving interrupts
ao2 = Auxiliary output 2, interrupt গ্রহণের জন্য ব্যবহৃত

ao1 Auxiliary output 1
ao1 = Auxiliary output 1

rts Ready To Transmit
rts = পাঠানোর জন্য প্রস্তুত

dtr Data Terminal Ready
dtr = ডাটা টার্মিনাল প্রস্তুত

---

We don’t need to enable interrupts, because we won’t handle any received data.
আমাদের interrupts চালু করার দরকার নেই, কারণ আমরা কোনো প্রাপ্ত ডাটা (received data) পরিচালনা করব না।

Therefore we use the configuration value 0x03 = 00000011 (RTS = 1 and DTS = 1).
সুতরাং আমরা কনফিগারেশন মান হিসেবে 0x03 = 00000011 ব্যবহার করি (RTS = 1 এবং DTR = 1)।

---

## Writing Data to the Serial Port

**সিরিয়াল পোর্টে ডাটা লেখা**

Writing data to the serial port is done via the data I/O port.
সিরিয়াল পোর্টে ডাটা লেখা হয় data I/O port-এর মাধ্যমে।

However, before writing, the transmit FIFO queue has to be empty (all previous writes must have finished).
তবে লেখার আগে transmit FIFO queue খালি থাকতে হবে (আগের সব লেখা শেষ হতে হবে)।

The transmit FIFO queue is empty if bit 5 of the line status I/O port is equal to one.
Line status I/O port-এর bit 5 যদি 1 হয়, তাহলে transmit FIFO queue খালি থাকে।

---

Reading the contents of an I/O port is done via the in assembly code instruction.
I/O port-এর কনটেন্ট পড়া হয় `in` assembly instruction ব্যবহার করে।

There is no way to use the in assembly code instruction from C, therefore it has to be wrapped (the same way as the out assembly code instruction):
`in` assembly instruction সরাসরি C থেকে ব্যবহার করা যায় না, তাই এটিকেও `out` instruction-এর মতো wrapper function আকারে লিখতে হয়:

---

global inb
inb কে global ঘোষণা করা হলো

; inb - returns a byte from the given I/O port
; inb - নির্দিষ্ট I/O port থেকে একটি byte ফেরত দেয়

; stack: [esp + 4] The address of the I/O port
; stack: [esp + 4] I/O port-এর ঠিকানা

; [esp ] The return address
; [esp ] return address

inb:

mov dx, [esp + 4] ; move the address of the I/O port to the dx register
mov dx, [esp + 4] ; I/O port-এর ঠিকানা dx register-এ নেওয়া হলো

in al, dx ; read a byte from the I/O port and store it in the al register
in al, dx ; I/O port থেকে একটি byte পড়ে al register-এ রাখা হলো

ret ; return the read byte
ret ; পড়া byte ফেরত দেওয়া হলো

---

/* in file io.h */
/* io.h ফাইলে */

/** inb:
/** inb:

* Read a byte from an I/O port.

* একটি I/O port থেকে একটি byte পড়ে

*

*

* @param port The address of the I/O port

* @param port I/O port-এর ঠিকানা

* @return The read byte

* @return পড়া byte

*/

unsigned char inb(unsigned short port);
unsigned char inb(unsigned short port);

---

Checking if the transmit FIFO is empty can then be done from C:
তারপর C থেকে transmit FIFO খালি কিনা তা পরীক্ষা করা যায়:

---

/** serial_is_transmit_fifo_empty:
/** serial_is_transmit_fifo_empty:

* Checks whether the transmit FIFO queue is empty or not for the given COM

* নির্দিষ্ট COM পোর্টের transmit FIFO queue খালি কিনা তা পরীক্ষা করে

* port.

* পোর্টের জন্য

*

*

* @param com The COM port

* @param com COM পোর্ট

* @return 0 if the transmit FIFO queue is not empty

* @return transmit FIFO queue খালি না হলে 0 ফেরত দেয়

* 1 if the transmit FIFO queue is empty

* transmit FIFO queue খালি হলে 1 ফেরত দেয়

*/

int serial_is_transmit_fifo_empty(unsigned int com)

{

/* 0x20 = 0010 0000 */
/* 0x20 = 0010 0000 */

return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;

}

---

Writing to a serial port means spinning as long as the transmit FIFO queue isn’t empty, and then writing the data to the data I/O port.
সিরিয়াল পোর্টে লেখা মানে হলো transmit FIFO queue খালি না হওয়া পর্যন্ত অপেক্ষা করা (spin করা), তারপর data I/O port-এ ডাটা লেখা।

---

## Configuring Bochs

**Bochs কনফিগার করা**

To save the output from the first serial serial port the Bochs configuration file bochsrc.txt must be updated.
প্রথম সিরিয়াল পোর্টের আউটপুট সংরক্ষণ করতে Bochs-এর কনফিগারেশন ফাইল bochsrc.txt আপডেট করতে হবে।

The com1 configuration instructs Bochs how to handle first serial port:
com1 কনফিগারেশন Bochs-কে জানায় প্রথম সিরিয়াল পোর্ট কীভাবে পরিচালনা করবে:

com1: enabled=1, mode=file, dev=com1.out
com1: enabled=1, mode=file, dev=com1.out

The output from serial port one will now be stored in the file com1.out.
এখন প্রথম সিরিয়াল পোর্টের আউটপুট com1.out ফাইলে সংরক্ষিত হবে।

---

## The Driver

**ড্রাইভার**

We recommend that you implement a write function for the serial port similar to the write function in the driver for the framebuffer.
আমরা পরামর্শ দিই যে framebuffer ড্রাইভারের write function-এর মতো সিরিয়াল পোর্টের জন্যও একটি write function তৈরি করো।

To avoid name clashes with the write function for the framebuffer it is a good idea to name the functions fb_write and serial_write to distinguish them.
Framebuffer-এর write function-এর সাথে নামের সংঘর্ষ এড়াতে ফাংশনগুলোর নাম fb_write এবং serial_write রাখা ভালো।

---

We further recommend that you try to write a printf-like function, see section 7.3 in [8].
আমরা আরও পরামর্শ দিই যে printf-এর মতো একটি function লেখার চেষ্টা করো, [8]-এর section 7.3 দেখো।

The printf function could take an additional argument to decide to which device to write the output (framebuffer or serial).
printf function একটি অতিরিক্ত argument নিতে পারে যাতে নির্ধারণ করা যায় আউটপুট framebuffer-এ যাবে নাকি serial পোর্টে যাবে।

---

A final recommendation is that you create some way of distinguishing the severeness of the log messages, for example by prepending the messages with DEBUG, INFO or ERROR.
শেষ পরামর্শ হলো log message-এর গুরুত্ব বোঝানোর জন্য কোনো ব্যবস্থা তৈরি করা, যেমন message-এর আগে DEBUG, INFO বা ERROR যোগ করা।

---

## Further Reading

**আরও পড়ার জন্য**

• The book “Serial programming” (available on WikiBooks) has a great section on programming the serial port,
• “Serial programming” বইটি (WikiBooks-এ পাওয়া যায়) সিরিয়াল পোর্ট প্রোগ্রামিং নিয়ে একটি ভালো অধ্যায় রয়েছে,

[http://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming#UART_Registers](http://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming#UART_Registers)

• The OSDev wiki has a page with a lot of information about the serial ports,
• OSDev wiki-তে সিরিয়াল পোর্ট সম্পর্কে অনেক তথ্যসহ একটি পেজ আছে,

[http://wiki.osdev.org/Serial_ports](http://wiki.osdev.org/Serial_ports)

---

Output
আউটপুট

Interacting with the Hardware
হার্ডওয়্যারের সাথে যোগাযোগ

The Framebuffer
ফ্রেমবাফার

Writing Text
টেক্সট লেখা

Moving the Cursor
কার্সর সরানো

The Driver
ড্রাইভার

The Serial Ports
সিরিয়াল পোর্টসমূহ

Configuring the Serial Port
সিরিয়াল পোর্ট কনফিগার করা

Configuring the Line
লাইন কনফিগার করা

Configuring the Buffers
বাফার কনফিগার করা

Configuring the Modem
মডেম কনফিগার করা

Writing Data to the Serial Port
সিরিয়াল পোর্টে ডাটা লেখা

Configuring Bochs
Bochs কনফিগার করা

The Driver
ড্রাইভার

Further Reading
আরও পড়ার জন্য
