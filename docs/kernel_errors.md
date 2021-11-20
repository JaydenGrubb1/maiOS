# Kernel Error Numbers

| No. | Hex | Error Name | Description |
| --- | --- | --- | ---|
| 0 | 0x00 | Unexpected Kernel Exit | Occurs if the main kernel function unexpectedly returns |
| 1 | 0x01 | No Multiboot Present | Occurs if the multiboot header is not present |
| 2 | 0x02 | No CPUID Instruction | Occurs if the CPU does not support the CPUID instruction |
| 3 | 0x03 | No Extended CPU Info | Occurs if the CPU does not support extended CPU info |
| 4 | 0x04 | No Long Mode | Occurs if the CPU does not support long mode |