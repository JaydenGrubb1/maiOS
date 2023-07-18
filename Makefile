# Compilers
CC := tools/compiler/bin/x86_64-elf-gcc
LD := tools/compiler/bin/x86_64-elf-ld
ASM := nasm

# Flags
ARCH := x86_64
CC_FLAGS := -std=c++20 -Wall -g -ffreestanding -Wextra -masm=intel -O2 \
	-fno-exceptions -fno-rtti -nostdlib -lgcc -mgeneral-regs-only -mno-red-zone \
	-fno-use-cxa-atexit
# TODO apply -mgeneral-regs-only and -mno-red-zone to relevant files only
# TODO add exception support
# VERIFY is -fno-use-cxa-atexit a valid solution
LD_FLAGS := 
QEMU_FLAGS := -m 128M -serial stdio

# Directories
BUILD_DIR := build
TARGET_DIR := $(BUILD_DIR)/targets
ISO_DIR := $(BUILD_DIR)/iso
INCLUDE_DIR := include
SRC_DIR := src
LINKER := $(SRC_DIR)/kernel/arch/$(ARCH)/linker.ld

# Global constructor objects
CRTI_SRC := $(SRC_DIR)/kernel/arch/$(ARCH)/crt/crti.asm
CRTI_OBJ := $(TARGET_DIR)/kernel/arch/$(ARCH)/crt/crti.o
CRTN_SRC := $(SRC_DIR)/kernel/arch/$(ARCH)/crt/crtn.asm
CRTN_OBJ := $(TARGET_DIR)/kernel/arch/$(ARCH)/crt/crtn.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CC_FLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CC_FLAGS) -print-file-name=crtend.o)

# Assembly objects
ASM_SRC := $(shell find $(SRC_DIR)/kernel/arch/$(ARCH)/boot -name *.asm)
ASM_OBJ := $(patsubst $(SRC_DIR)/kernel/arch/$(ARCH)/boot/%.asm, $(TARGET_DIR)/kernel/arch/$(ARCH)/boot/%.o, $(ASM_SRC))

# Kernel objects
KERNEL_SRC := $(shell find $(SRC_DIR)/kernel -name *.cpp ! -name *interrupts.cpp)
KERNEL_OBJ := $(patsubst $(SRC_DIR)/kernel/%.cpp, $(TARGET_DIR)/kernel/%.o, $(KERNEL_SRC))

# Interrupt objects
INTERRUPTS_SRC := $(SRC_DIR)/kernel/arch/$(ARCH)/interrupts.cpp
INTERRUPTS_OBJ := $(patsubst $(SRC_DIR)/kernel/%.cpp, $(TARGET_DIR)/kernel/%.o, $(INTERRUPTS_SRC))

# Library objects
LIB_SRC := $(shell find $(SRC_DIR)/lib -name *.cpp)
LIB_OBJ := $(patsubst $(SRC_DIR)/lib/%.cpp, $(TARGET_DIR)/lib/%.o, $(LIB_SRC))

# Headers
HEADERS := $(shell find $(INCLUDE_DIR) -name *.h)

# Default target
all: run

# Compiles global constructor objects
$(CRTI_OBJ): $(CRTI_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(CRTI_SRC) -o $@
$(CRTN_OBJ): $(CRTN_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(CRTN_SRC) -o $@

# Compiles all assembly objects
$(ASM_OBJ): $(ASM_SRC)
	mkdir -p $(dir $@) && \
	$(ASM) -f elf64 $(patsubst $(TARGET_DIR)/kernel/arch/$(ARCH)/boot/%.o, $(SRC_DIR)/kernel/arch/$(ARCH)/boot/%.asm, $@) -o $@

# Compiles all kernel objects
$(KERNEL_OBJ): $(KERNEL_SRC) $(HEADERS)
	mkdir -p $(dir $@) && \
	$(CC) -x c++ -c $(CC_FLAGS) -D __is_kernel -D __arch_$(ARCH) -I $(INCLUDE_DIR) $(patsubst $(TARGET_DIR)/kernel/%.o, $(SRC_DIR)/kernel/%.cpp, $@) -o $@

# Compiles all interrupt objects
$(INTERRUPTS_OBJ): $(INTERRUPTS_SRC) $(HEADERS)
	mkdir -p $(dir $@) && \
	$(CC) -x c++ -c $(CC_FLAGS) -mgeneral-regs-only -D __is_kernel -D __arch_$(ARCH) -I $(INCLUDE_DIR) $(patsubst $(TARGET_DIR)/kernel/%.o, $(SRC_DIR)/kernel/%.cpp, $@) -o $@

# Compiles all library objects
$(LIB_OBJ): $(LIB_SRC)
	mkdir -p $(dir $@) && \
	$(CC) -x c++ -c $(CC_FLAGS) -D __is_kernel -D __arch_$(ARCH) -I $(INCLUDE_DIR) $(patsubst $(TARGET_DIR)/lib/%.o, $(SRC_DIR)/lib/%.cpp, $@) -o $@

# List of all objects to be linked
LINK_LIST := \
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(ASM_OBJ) \
$(KERNEL_OBJ) \
$(INTERRUPTS_OBJ) \
$(LIB_OBJ) \
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