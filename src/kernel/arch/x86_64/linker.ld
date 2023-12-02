OUTPUT_FORMAT(elf64-x86-64)
ENTRY(init32_start)

SECTIONS
{
	. = 1M;

	__kernel_start = .;

	.text ALIGN(4K) :
	{
		__kernel_text_start = .;
		*(.multiboot)
		*(.text*)
		__kernel_text_end = .;
	}

	.rodata ALIGN(4K) :
	{
		__kernel_rodata_start = .;
		*(.rodata*)
		__kernel_rodata_end = .;
	}

	.data ALIGN(4K) :
	{
		__kernel_data_start = .;
		*(.data*)
		__kernel_data_end = .;
	}

	.bss ALIGN(4K) (NOLOAD) :
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