# Compilers
CC := tools/compiler/bin/x86_64-elf-gcc
LD := tools/compiler/bin/x86_64-elf-ld
ASM := nasm

# Flags
C_FLAGS := -std=c17 -Wall -g -ffreestanding
CPP_FLAGS := -std=c++20 -Wall -g -ffreestanding -fno-exceptions
# TODO Add exception support
LD_FLAGS := 
QEMU_FLAGS := -m 128M -serial stdio

# Directories
BUILD_DIR := build
TARGET_DIR := $(BUILD_DIR)/targets
ISO_DIR := $(BUILD_DIR)/iso
INCLUDE_DIR := include
LINKER := kernel/linker.ld

# Assembly objects
ASM_SRC := $(shell find kernel/boot -name *.asm)
ASM_OBJ := $(patsubst kernel/boot/%.asm, $(TARGET_DIR)/kernel/boot/%.o, $(ASM_SRC))

# C objects
C_SRC := $(shell find kernel -name *.c)
C_OBJ := $(patsubst kernel/%.c, $(TARGET_DIR)/kernel/%.o, $(C_SRC))

# CPP objects
CPP_SRC := $(shell find kernel -name *.cpp)
CPP_OBJ := $(patsubst kernel/%.cpp, $(TARGET_DIR)/kernel/%.o, $(CPP_SRC))

# Headers
HEADERS := $(shell find include -name *.h)

# Default target
all: build

# Compiles all the kernel assembly objects
$(ASM_OBJ): $(ASM_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(patsubst $(TARGET_DIR)/kernel/boot/%.o, kernel/boot/%.asm, $@) -o $@

# Compiles all the kernel c objects
$(C_OBJ): $(C_SRC)
	mkdir -p $(dir $@) && \
	$(CC) -c $(C_FLAGS) $(patsubst $(TARGET_DIR)/kernel/%.o, kernel/%.c, $@) -o $@

# Compiles all the kernel cpp objects
$(CPP_OBJ): $(CPP_SRC) $(HEADERS)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CPP_FLAGS) -I $(INCLUDE_DIR) $(patsubst $(TARGET_DIR)/kernel/%.o, kernel/%.cpp, $@) -o $@

# Links all the objects into a binary
$(ISO_DIR)/boot/kernel.bin: $(ASM_OBJ) $(C_OBJ) $(CPP_OBJ)
	mkdir -p $(ISO_DIR)/boot && \
	$(LD) -n -o $(ISO_DIR)/boot/kernel.bin $(LD_FLAGS) -T $(LINKER) $(ASM_OBJ) $(CPP_OBJ)

# Copies GRUP config to ISO directory
$(ISO_DIR): $(ISO_DIR)/boot/kernel.bin
	cp -r conf/grub $(ISO_DIR)/boot/grub

# Makes an ISO image from the ISO directory
$(BUILD_DIR)/kernel.iso: $(ISO_DIR) $(ISO_DIR)/boot/kernel.bin
	grub-mkrescue -o $(BUILD_DIR)/kernel.iso $(ISO_DIR)

# PHONY targets
build: $(BUILD_DIR)/kernel.iso
run: build
	qemu-system-x86_64 -cdrom ./build/kernel.iso $(QEMU_FLAGS)
debug: build
	qemu-system-x86_64 -cdrom ./build/kernel.iso $(QEMU_FLAGS) -S -gdb tcp::1234
clean:
	rm -rf $(BUILD_DIR)
.PHONY: clean run build all