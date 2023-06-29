# Compilers
CC := tools/compiler/bin/x86_64-elf-gcc
LD := tools/compiler/bin/x86_64-elf-ld
ASM := nasm

# Flags
C_FLAGS := -std=c17 -Wall -g -ffreestanding -masm=intel -O2
CPP_FLAGS := -std=c++20 -Wall -g -ffreestanding -masm=intel -O2 \
	-fno-exceptions -fno-rtti -nostdlib -lgcc
# TODO Add exception support
LD_FLAGS := 
QEMU_FLAGS := -m 128M -serial stdio

# Directories
BUILD_DIR := build
TARGET_DIR := $(BUILD_DIR)/targets
ISO_DIR := $(BUILD_DIR)/iso
INCLUDE_DIR := include
LINKER := kernel/linker.ld

# Global constructor objects
CRTI_SRC := kernel/crt/crti.asm
CRTI_OBJ := $(TARGET_DIR)/kernel/crt/crti.o
CRTN_SRC := kernel/crt/crtn.asm
CRTN_OBJ := $(TARGET_DIR)/kernel/crt/crtn.o
CRTBEGIN_OBJ:=$(shell $(CC) $(C_FLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(C_FLAGS) -print-file-name=crtend.o)

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
HEADERS := $(shell find include -name *.h -o -name *.hpp)

# Default target
all: run

# Compiles global constructor objects
$(CRTI_OBJ): $(CRTI_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(CRTI_SRC) -o $@
$(CRTN_OBJ): $(CRTN_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(CRTN_SRC) -o $@

# Compiles all the kernel assembly objects
$(ASM_OBJ): $(ASM_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(patsubst $(TARGET_DIR)/kernel/boot/%.o, kernel/boot/%.asm, $@) -o $@

# Compiles all the kernel c objects
$(C_OBJ): $(C_SRC)
	mkdir -p $(dir $@) && \
	$(CC) -c $(C_FLAGS) -I $(INCLUDE_DIR) $(patsubst $(TARGET_DIR)/kernel/%.o, kernel/%.c, $@) -o $@

# Compiles all the kernel cpp objects
$(CPP_OBJ): $(CPP_SRC) $(HEADERS)
	mkdir -p $(dir $@) && \
	$(CC) -c $(CPP_FLAGS) -I $(INCLUDE_DIR) $(patsubst $(TARGET_DIR)/kernel/%.o, kernel/%.cpp, $@) -o $@

# List of all objects to be linked
LINK_LIST := \
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(ASM_OBJ) \
$(C_OBJ) \
$(CPP_OBJ) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \

# Links all the objects into a binary
$(ISO_DIR)/boot/kernel.bin: $(LINK_LIST) $(LINKER)
	mkdir -p $(ISO_DIR)/boot && \
	$(LD) -n -o $(ISO_DIR)/boot/kernel.bin $(LD_FLAGS) -T $(LINKER) $(LINK_LIST)

# Copies GRUP config to ISO directory
$(ISO_DIR): $(ISO_DIR)/boot/kernel.bin
	cp -r conf/grub $(ISO_DIR)/boot

# Makes an ISO image from the ISO directory
$(BUILD_DIR)/kernel.iso: $(ISO_DIR) $(ISO_DIR)/boot/kernel.bin
	grub-mkrescue -o $(BUILD_DIR)/kernel.iso $(ISO_DIR)

# PHONY targets
build: $(BUILD_DIR)/kernel.iso
new: clean | build
run: build
	qemu-system-x86_64 -cdrom ./build/kernel.iso $(QEMU_FLAGS)
debug: build
	qemu-system-x86_64 -cdrom ./build/kernel.iso $(QEMU_FLAGS) -S -gdb tcp::1234
clean:
	rm -rf $(BUILD_DIR)
.PHONY: clean run build all