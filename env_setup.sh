#!/bin/bash

# ===== Color codes =====
GREEN="\e[32m"
BLUE="\e[34m"
YELLOW="\e[33m"
RED="\e[31m"
RESET="\e[0m"

echo -e "${BLUE}🔄 Updating package list...${RESET}"
sudo apt update

echo -e "${BLUE}⬆️  Upgrading installed packages...${RESET}"
sudo apt upgrade -y

echo -e "${YELLOW}🛠 Installing required tools for low-level / OS development...${RESET}"

# GCC → C compiler (kernel / low-level code compile করার জন্য)
echo -e "${GREEN}➡ Installing GCC (C compiler)...${RESET}"
sudo apt install -y gcc

# NASM → Assembly code লেখার জন্য
echo -e "${GREEN}➡ Installing NASM (Assembly compiler)...${RESET}"
sudo apt install -y nasm

# Make → build automation (Makefile চালানোর জন্য)
echo -e "${GREEN}➡ Installing Make (build tool)...${RESET}"
sudo apt install -y make

# QEMU → Virtual machine / OS emulator
echo -e "${GREEN}➡ Installing QEMU (x86 system emulator)...${RESET}"
sudo apt install -y qemu-system-x86

echo -e "${BLUE}🔍 Verifying installations...${RESET}"

gcc --version | head -n 1
nasm -v
ld --version | head -n 1
make --version | head -n 1
qemu-system-x86_64 --version | head -n 1

echo -e "${GREEN}✅ All tools installed successfully!${RESET}"
echo -e "${BLUE}🎉 Your environment is ready for OS development!${RESET}"