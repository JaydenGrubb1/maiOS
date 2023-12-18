OUTPUT_FORMAT(elf64-x86-64)
ENTRY(init32_start)

KERNEL_PHYS = 0x200000;
KERNEL_VIRT = 0xffffffff80000000;

SECTIONS
{
	. = KERNEL_PHYS + KERNEL_VIRT;

	__kernel_start = .;

	.text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRT)
	{
		__kernel_text_start = .;
		*(.multiboot)
		*(.text*)
		__kernel_text_end = .;
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRT)
	{
		__kernel_rodata_start = .;
		*(.rodata*)
		__kernel_rodata_end = .;
	}

	.data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRT)
	{
		__kernel_data_start = .;
		*(.data*)
		__kernel_data_end = .;
	}

	.bss ALIGN(4K) (NOLOAD) : AT(ADDR(.bss) - KERNEL_VIRT)
	{
		__kernel_bss_start = .;
		*(COMMON)
		*(.bss*)
		__kernel_bss_end = .;

		. = ALIGN(4K);
		__kernel_stack_start = .;
		*(.stack)
		__kernel_stack_end = .;

		. = ALIGN(4K);
		__page_tables_start = .;
		*(.page_tables)
		__page_tables_end = .;

		. = ALIGN(4K);
		__kernel_heap_start = .;
		*(.heap)
		__kernel_heap_end = .;
	}

	__kernel_end = .;

	/DISCARD/ :
	{
		*(.note*)
		*(.comment*)
	}
}