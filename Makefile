CC := tools/compiler/bin/x86_64-elf-gcc
LD := tools/compiler/bin/x86_64-elf-ld
ASM := nasm

CFLAGS := -std=c17 -Wall -pedantic -g
CPPFLAGS := -std=c++20 -Wall -pedantic -g
LDFLAGS := -g
LINKER := kernel/linker.ld

BUILD_DIR := build
TARGET_DIR := $(BUILD_DIR)/targets
ISO_DIR := $(BUILD_DIR)/iso

ASM_SRC := $(shell find kernel/boot -name *.asm)
ASM_OBJ := $(patsubst kernel/boot/%.asm, $(TARGET_DIR)/kernel/boot/%.o, $(ASM_SRC))

all: build

$(ASM_OBJ): $(ASM_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(patsubst $(TARGET_DIR)/kernel/boot/%.o, kernel/boot/%.asm, $@) -o $@

$(ISO_DIR)/boot/kernel.bin: $(ASM_OBJ)
	mkdir -p $(ISO_DIR)/boot && \
	$(LD) -n -o $(ISO_DIR)/boot/kernel.bin -T $(LINKER) $(ASM_OBJ)

$(ISO_DIR): $(ISO_DIR)/boot/kernel.bin
	cp -r conf/grub $(ISO_DIR)/boot/grub && \
	grub-mkrescue -o $(BUILD_DIR)/kernel.iso $(ISO_DIR)

$(BUILD_DIR)/kernel.iso: $(ISO_DIR) $(ISO_DIR)

build: $(BUILD_DIR)/kernel.iso

run: build
	qemu-system-x86_64 -cdrom ./build/kernel.iso

debug: build
	qemu-system-x86_64 -cdrom ./build/kernel.iso -S -gdb tcp::1234

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean run build all