// kernel/kernel.c

#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x0F
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

int cursor_position = 0;

// screen clear করে
void clear_screen() {
    char* video_memory = (char*) VIDEO_MEMORY;

    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = WHITE_ON_BLACK;
    }

    // cursor reset
    cursor_position = 0;
}

// newline handle করে
void new_line() {
    int current_line = cursor_position / SCREEN_WIDTH;
    cursor_position = (current_line + 1) * SCREEN_WIDTH;
}

// একটা character print করে
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

// string print করে
void print(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i]);
        i++;
    }
}

// kernel entry point
void kernel_main() {
    clear_screen();
    print("MiniOS is running...\n");
    print("Welcome to your first OS!");
}
