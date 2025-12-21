// kernel/kernel.c

void kernel_main() {
    char* video_memory = (char*) 0xB8000;

    video_memory[0] = 'M';
    video_memory[1] = 0x0F;

    video_memory[2] = 'i';
    video_memory[3] = 0x0F;

    video_memory[4] = 'n';
    video_memory[5] = 0x0F;

    video_memory[6] = 'i';
    video_memory[7] = 0x0F;

    video_memory[8] = 'O';
    video_memory[9] = 0x0F;

    video_memory[10] = 'S';
    video_memory[11] = 0x0F;

    video_memory[12] = ' ';
    video_memory[13] = 0x0F;

    video_memory[14] = 'i';
    video_memory[15] = 0x0F;

    video_memory[16] = 's';
    video_memory[17] = 0x0F;

    video_memory[18] = ' ';
    video_memory[19] = 0x0F;

    video_memory[20] = 'r';
    video_memory[21] = 0x0F;

    video_memory[22] = 'u';
    video_memory[23] = 0x0F;

    video_memory[24] = 'n';
    video_memory[25] = 0x0F;

    video_memory[26] = 'n';
    video_memory[27] = 0x0F;

    video_memory[28] = 'i';
    video_memory[29] = 0x0F;

    video_memory[30] = 'n';
    video_memory[31] = 0x0F;

    video_memory[32] = 'g';
    video_memory[33] = 0x0F;

    video_memory[34] = '.';
    video_memory[35] = 0x0F;

    video_memory[36] = '.';
    video_memory[37] = 0x0F;

    video_memory[38] = '.';
    video_memory[39] = 0x0F;
}
