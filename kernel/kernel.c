#include "memory.h"

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int cursor_position = 0;
extern char _kernel_end;

void clear_screen() {
    char* video_memory = (char*) VIDEO_MEMORY;

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = WHITE_ON_BLACK;
    }

    // cursor reset
    cursor_position = 0;
}

void new_line() {
    int current_line = cursor_position / SCREEN_WIDTH;
    cursor_position = (current_line + 1) * SCREEN_WIDTH;
}

void print_char(char c) {
    if (c == '\n') {
        new_line();
        return;
    }

    char* video_memory = (char*) VIDEO_MEMORY;
    video_memory[cursor_position * 2] = c;
    video_memory[cursor_position * 2 + 1] = WHITE_ON_BLACK;
    cursor_position++;
}

void print_hex(unsigned int num) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[9]; // 8 hex digits + null
    buffer[8] = '\0';

    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }

    print("0x");
    print(buffer);
}

void print(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i]);
        i++;
    }
}

void kernel_main() {
    clear_screen();
    print("MiniOS is running...\n");
    print("Welcome to your first OS!\n");
    print("Initializing memory...\n");
    init_memory();

    print("Memory initialized!\n");
    char* ch = (char*) kmalloc(10);
    ch[0] = 'O';
    ch[1] = 'k';
    ch[2] = '\0';
    print(ch);
    new_line();

    uint32_t a = (uint32_t) kmalloc(1);
    uint32_t b = (uint32_t) kmalloc(1);
    print_hex(a);
    new_line();
    print_hex(b);
    new_line();
}
