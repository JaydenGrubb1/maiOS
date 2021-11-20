CC := tools/compiler/bin/x86_64-elf-gcc
LD := tools/compiler/bin/x86_64-elf-ld
ASM := nasm

CFLAGS := -std=c17 -Wall -pedantic -g
CPPFLAGS := -std=c++20 -Wall -pedantic -g
LDFLAGS := -g
LINKER := kernel/linker.ld

BUILD_DIR := build
TARGET_DIR := $(BUILD_DIR)/targets
BIN_DIR := $(BUILD_DIR)/bin
ISO_DIR := $(BUILD_DIR)/iso

# $(BUILD_DIR):
# 	mkdir $(BUILD_DIR)
# $(BIN_DIR): $(BUILD_DIR)
# 	mkdir $(BIN_DIR)
# $(TARGET_DIR): $(BUILD_DIR)
# 	mkdir $(TARGET_DIR)

ASM_SRC := $(shell find kernel/boot -name *.asm)
ASM_OBJ := $(patsubst kernel/boot/%.asm, $(TARGET_DIR)/kernel/boot/%.o, $(ASM_SRC))

$(ASM_OBJ): $(ASM_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(patsubst $(TARGET_DIR)/kernel/boot/%.o, kernel/boot/%.asm, $@) -o $@

$(BIN_DIR)/kernel.bin: $(ASM_OBJ)
	mkdir -p $(BIN_DIR) && \
	$(LD) -n -o $(BIN_DIR)/kernel.bin -T $(LINKER) $(ASM_OBJ)

binary: $(BIN_DIR)/kernel.bin

$(ISO_DIR): $(BIN_DIR)/kernel.bin
	mkdir -p $(ISO_DIR)/boot && \
	cp $(BIN_DIR)/kernel.bin $(ISO_DIR)/boot/kernel.bin && \
	cp -r conf/grub $(ISO_DIR)/boot/grub && \
	grub-mkrescue -o $(BUILD_DIR)/kernel.iso $(ISO_DIR)
# grub-mkrescue /usr/lib/grub/i386-pc -o $(BUILD_DIR)/kernel.iso $(ISO_DIR)

$(BUILD_DIR)/kernel.iso: $(ISO_DIR)

image: $(BUILD_DIR)/kernel.iso

# kernel_source_files := $(shell find src/impl/kernel -name *.cpp)
# kernel_object_files := $(patsubst src/impl/kernel/%.cpp, build/kernel/%.o, $(kernel_source_files))

# x86_64_c_source_files := $(shell find src/impl/x86_64 -name *.cpp)
# x86_64_c_object_files := $(patsubst src/impl/x86_64/%.cpp, build/x86_64/%.o, $(x86_64_c_source_files))

# x86_64_asm_source_files := $(shell find src/impl/x86_64 -name *.asm)
# x86_64_asm_object_files := $(patsubst src/impl/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

# x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)

# all: build-x86_64

# $(kernel_object_files): build/kernel/%.o : src/impl/kernel/%.cpp
# 	mkdir -p $(dir $@) && \
# 	$(CC) -c $(CFLAGS) -I src/intf -ffreestanding $(patsubst build/kernel/%.o, src/impl/kernel/%.cpp, $@) -o $@

# $(x86_64_c_object_files): build/x86_64/%.o : src/impl/x86_64/%.cpp
# 	mkdir -p $(dir $@) && \
# 	$(CC) -c $(CFLAGS) -I src/intf -ffreestanding $(patsubst build/x86_64/%.o, src/impl/x86_64/%.cpp, $@) -o $@

# $(x86_64_asm_object_files): build/x86_64/%.o : src/impl/x86_64/%.asm
# 	mkdir -p $(dir $@) && \
# 	nasm -f elf64 $(patsubst build/x86_64/%.o, src/impl/x86_64/%.asm, $@) -o $@

# .PHONY: build-x86_64
# build-x86_64: $(kernel_object_files) $(x86_64_object_files)
# 	mkdir -p dist/x86_64 && \
# 	$(LD) -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(kernel_object_files) $(x86_64_object_files) && \
# 	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
# 	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso

# cmd.exe /C qemu-system-x86_64 -cdrom build/kernel.iso
run:
	qemu-system-x86_64 -cdrom ./build/kernel.iso

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean run image binary