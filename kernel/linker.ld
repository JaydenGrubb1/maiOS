ENTRY(init32_start)

SECTIONS
{
	. = 1M;

	.text : AT(ADDR(.text))
	{
		*(.multiboot*)
		*(.text*)
	}
}