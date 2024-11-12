# Interrupts Mapping

Interrupts with vector 0 to 31 are reserved by the CPU and cannot be modified. Interrupts with vector 32 onwards can be modified using `Interrupts::set_isr()` and `Interrupts::clear_isr()`. These interrupts are default initialized to `default_isr` in `kernel/arch/x86_64/interrupts.cpp`. The table below lists the current mapping of interrupts.

| Vector | Description | Type | IRQ | Handler | Location |
| --- | --- | --- | --- | --- | --- |
| 0 | Divide by Zero | Exception (fault) | | `division_error` | `kernel/arch/x86_64/interrupts.cpp` |
| 1 | Debug | Exception (fault/trap) | | `debug` | `kernel/arch/x86_64/interrupts.cpp` |
| 2 | Non-maskable Interrupt | Interrupt | | `non_maskable` | `kernel/arch/x86_64/interrupts.cpp` |
| 3 | Breakpoint | Exception (trap) | | `breakpoint` | `kernel/arch/x86_64/interrupts.cpp` |
| 4 | Overflow | Exception (trap) | | `overflow` | `kernel/arch/x86_64/interrupts.cpp` |
| 5 | Bound Range Exceeded | Exception (fault) | | | |
| 6 | Invalid Opcode | Exception (fault) | | `invalid_opcode` | `kernel/arch/x86_64/interrupts.cpp` |
| 7 | Device Not Available | Exception (fault) | | `device_not_available` | `kernel/arch/x86_64/interrupts.cpp` |
| 8 | Double Fault | Exception (abort) | | `double_fault` | `kernel/arch/x86_64/interrupts.cpp` |
| 9 | ***DEPRECATED*** | | | | |
| 10 | Invalid TSS | Exception (fault) | | `invalid_tss` | `kernel/arch/x86_64/interrupts.cpp` |
| 11 | Segment Not Present | Exception (fault) | | `segment_not_present` | `kernel/arch/x86_64/interrupts.cpp` |
| 12 | Stack-segment Fault | Exception (fault) | | `stack_segment_fault` | `kernel/arch/x86_64/interrupts.cpp` |
| 13 | General Protection Fault | Exception (fault) | | `general_protection_fault` | `kernel/arch/x86_64/interrupts.cpp` |
| 14 | Page Fault | Exception (fault) | | `page_fault` | `kernel/arch/x86_64/interrupts.cpp` |
| 15 | ***RESERVED*** | | | | |
| 16 | FPU Floating-point Error | Exception (fault) | | `fpu_floating_point_error` | `kernel/arch/x86_64/interrupts.cpp` |
| 17 | Alignment Check | Exception (fault) | | `alignment_check` | `kernel/arch/x86_64/interrupts.cpp` |
| 18 | Machine Check | Exception (abort) | | `machine_check` | `kernel/arch/x86_64/interrupts.cpp` |
| 19 | SIMD Floating-point Exception | Exception (fault) | | `simd_floating_point_error` | `kernel/arch/x86_64/interrupts.cpp` |
| 20 | Virtualization Exception | Exception (fault) | | `virtualization_error` | `kernel/arch/x86_64/interrupts.cpp` |
| 21 | Control Protection Exception | Exception (fault) | | `control_protection_exception` | `kernel/arch/x86_64/interrupts.cpp` |
| 22 | ***RESERVED*** | | | | |
| 23 | ***RESERVED*** | | | | |
| 24 | ***RESERVED*** | | | | |
| 25 | ***RESERVED*** | | | | |
| 26 | ***RESERVED*** | | | | |
| 27 | ***RESERVED*** | | | | |
| 28 | Hypervisor Injection Exception | Exception (fault) | | | |
| 29 | VMM Communication Exception | Exception (fault) | | | |
| 30 | Security Exception | Exception (fault) | | | |
| 31 | ***RESERVED*** | | | | |
| 32 | System Timer | IRQ | 0 | `scheduler_preempt` | `kernel/arch/x86_64/asm/scheduler.asm` |
| 33 | Keyboard | IRQ | 1 | | |
| 34 | Cascade (used internally by PIC) | IRQ | 2 | | |
| 35 | COM2 | IRQ | 3 | | |
| 36 | COM1 | IRQ | 4 | | |
| 37 | LPT2 | IRQ | 5 | | |
| 38 | Floppy Disk | IRQ | 6 | | |
| 39 | LPT1 / Spurious Interrupt | IRQ | 7 | | |
| 40 | CMOS Real-time Clock | IRQ | 8 | | |
| 41 | Free for Peripherals / Legacy SCSI / NIC | IRQ | 9 | | |
| 42 | Free for Peripherals / SCSI / NIC | IRQ | 10 | | |
| 43 | Free for Peripherals / SCSI / NIC | IRQ | 11 | | |
| 44 | PS/2 Mouse | IRQ | 12 | | |
| 45 | FPU / Coprocessor / Inter-processor | IRQ | 13 | | |
| 46 | Primary ATA Hard Disk | IRQ | 14 | | |
| 47 | Secondary ATA Hard Disk | IRQ | 15 | | |
| 48 | Scheduler Yield | Interrupt | | `scheduler_yield` | `kernel/arch/x86_64/asm/scheduler.asm` |
| 49 | ***AVAILABLE*** | | | | |
| 50 | ***AVAILABLE*** | | | | |
| 51 | ***AVAILABLE*** | | | | |
| 52 | ***AVAILABLE*** | | | | |
| 53 | ***AVAILABLE*** | | | | |
| 54 | ***AVAILABLE*** | | | | |
| 55 | ***AVAILABLE*** | | | | |
| 56 | ***AVAILABLE*** | | | | |
| 57 | ***AVAILABLE*** | | | | |
| 58 | ***AVAILABLE*** | | | | |
| 59 | ***AVAILABLE*** | | | | |
| 60 | ***AVAILABLE*** | | | | |
| 61 | ***AVAILABLE*** | | | | |
| 62 | ***AVAILABLE*** | | | | |
| 63 | ***AVAILABLE*** | | | | |
| 64 | ***AVAILABLE*** | | | | |
| 65 | ***AVAILABLE*** | | | | |
| 66 | ***AVAILABLE*** | | | | |
| 67 | ***AVAILABLE*** | | | | |
| 68 | ***AVAILABLE*** | | | | |
| 69 | ***AVAILABLE*** | | | | |
| 70 | ***AVAILABLE*** | | | | |
| 71 | ***AVAILABLE*** | | | | |
| 72 | ***AVAILABLE*** | | | | |
| 73 | ***AVAILABLE*** | | | | |
| 74 | ***AVAILABLE*** | | | | |
| 75 | ***AVAILABLE*** | | | | |
| 76 | ***AVAILABLE*** | | | | |
| 77 | ***AVAILABLE*** | | | | |
| 78 | ***AVAILABLE*** | | | | |
| 79 | ***AVAILABLE*** | | | | |
| 80 | ***AVAILABLE*** | | | | |
| 81 | ***AVAILABLE*** | | | | |
| 82 | ***AVAILABLE*** | | | | |
| 83 | ***AVAILABLE*** | | | | |
| 84 | ***AVAILABLE*** | | | | |
| 85 | ***AVAILABLE*** | | | | |
| 86 | ***AVAILABLE*** | | | | |
| 87 | ***AVAILABLE*** | | | | |
| 88 | ***AVAILABLE*** | | | | |
| 89 | ***AVAILABLE*** | | | | |
| 90 | ***AVAILABLE*** | | | | |
| 91 | ***AVAILABLE*** | | | | |
| 92 | ***AVAILABLE*** | | | | |
| 93 | ***AVAILABLE*** | | | | |
| 94 | ***AVAILABLE*** | | | | |
| 95 | ***AVAILABLE*** | | | | |
| 96 | ***AVAILABLE*** | | | | |
| 97 | ***AVAILABLE*** | | | | |
| 98 | ***AVAILABLE*** | | | | |
| 99 | ***AVAILABLE*** | | | | |
| 100 | ***AVAILABLE*** | | | | |
| 101 | ***AVAILABLE*** | | | | |
| 102 | ***AVAILABLE*** | | | | |
| 103 | ***AVAILABLE*** | | | | |
| 104 | ***AVAILABLE*** | | | | |
| 105 | ***AVAILABLE*** | | | | |
| 106 | ***AVAILABLE*** | | | | |
| 107 | ***AVAILABLE*** | | | | |
| 108 | ***AVAILABLE*** | | | | |
| 109 | ***AVAILABLE*** | | | | |
| 110 | ***AVAILABLE*** | | | | |
| 111 | ***AVAILABLE*** | | | | |
| 112 | ***AVAILABLE*** | | | | |
| 113 | ***AVAILABLE*** | | | | |
| 114 | ***AVAILABLE*** | | | | |
| 115 | ***AVAILABLE*** | | | | |
| 116 | ***AVAILABLE*** | | | | |
| 117 | ***AVAILABLE*** | | | | |
| 118 | ***AVAILABLE*** | | | | |
| 119 | ***AVAILABLE*** | | | | |
| 120 | ***AVAILABLE*** | | | | |
| 121 | ***AVAILABLE*** | | | | |
| 122 | ***AVAILABLE*** | | | | |
| 123 | ***AVAILABLE*** | | | | |
| 124 | ***AVAILABLE*** | | | | |
| 125 | ***AVAILABLE*** | | | | |
| 126 | ***AVAILABLE*** | | | | |
| 127 | ***AVAILABLE*** | | | | |
| 128 | ***AVAILABLE*** | | | | |
| 129 | ***AVAILABLE*** | | | | |
| 130 | ***AVAILABLE*** | | | | |
| 131 | ***AVAILABLE*** | | | | |
| 132 | ***AVAILABLE*** | | | | |
| 133 | ***AVAILABLE*** | | | | |
| 134 | ***AVAILABLE*** | | | | |
| 135 | ***AVAILABLE*** | | | | |
| 136 | ***AVAILABLE*** | | | | |
| 137 | ***AVAILABLE*** | | | | |
| 138 | ***AVAILABLE*** | | | | |
| 139 | ***AVAILABLE*** | | | | |
| 140 | ***AVAILABLE*** | | | | |
| 141 | ***AVAILABLE*** | | | | |
| 142 | ***AVAILABLE*** | | | | |
| 143 | ***AVAILABLE*** | | | | |
| 144 | ***AVAILABLE*** | | | | |
| 145 | ***AVAILABLE*** | | | | |
| 146 | ***AVAILABLE*** | | | | |
| 147 | ***AVAILABLE*** | | | | |
| 148 | ***AVAILABLE*** | | | | |
| 149 | ***AVAILABLE*** | | | | |
| 150 | ***AVAILABLE*** | | | | |
| 151 | ***AVAILABLE*** | | | | |
| 152 | ***AVAILABLE*** | | | | |
| 153 | ***AVAILABLE*** | | | | |
| 154 | ***AVAILABLE*** | | | | |
| 155 | ***AVAILABLE*** | | | | |
| 156 | ***AVAILABLE*** | | | | |
| 157 | ***AVAILABLE*** | | | | |
| 158 | ***AVAILABLE*** | | | | |
| 159 | ***AVAILABLE*** | | | | |
| 160 | ***AVAILABLE*** | | | | |
| 161 | ***AVAILABLE*** | | | | |
| 162 | ***AVAILABLE*** | | | | |
| 163 | ***AVAILABLE*** | | | | |
| 164 | ***AVAILABLE*** | | | | |
| 165 | ***AVAILABLE*** | | | | |
| 166 | ***AVAILABLE*** | | | | |
| 167 | ***AVAILABLE*** | | | | |
| 168 | ***AVAILABLE*** | | | | |
| 169 | ***AVAILABLE*** | | | | |
| 170 | ***AVAILABLE*** | | | | |
| 171 | ***AVAILABLE*** | | | | |
| 172 | ***AVAILABLE*** | | | | |
| 173 | ***AVAILABLE*** | | | | |
| 174 | ***AVAILABLE*** | | | | |
| 175 | ***AVAILABLE*** | | | | |
| 176 | ***AVAILABLE*** | | | | |
| 177 | ***AVAILABLE*** | | | | |
| 178 | ***AVAILABLE*** | | | | |
| 179 | ***AVAILABLE*** | | | | |
| 180 | ***AVAILABLE*** | | | | |
| 181 | ***AVAILABLE*** | | | | |
| 182 | ***AVAILABLE*** | | | | |
| 183 | ***AVAILABLE*** | | | | |
| 184 | ***AVAILABLE*** | | | | |
| 185 | ***AVAILABLE*** | | | | |
| 186 | ***AVAILABLE*** | | | | |
| 187 | ***AVAILABLE*** | | | | |
| 188 | ***AVAILABLE*** | | | | |
| 189 | ***AVAILABLE*** | | | | |
| 190 | ***AVAILABLE*** | | | | |
| 191 | ***AVAILABLE*** | | | | |
| 192 | ***AVAILABLE*** | | | | |
| 193 | ***AVAILABLE*** | | | | |
| 194 | ***AVAILABLE*** | | | | |
| 195 | ***AVAILABLE*** | | | | |
| 196 | ***AVAILABLE*** | | | | |
| 197 | ***AVAILABLE*** | | | | |
| 198 | ***AVAILABLE*** | | | | |
| 199 | ***AVAILABLE*** | | | | |
| 200 | ***AVAILABLE*** | | | | |
| 201 | ***AVAILABLE*** | | | | |
| 202 | ***AVAILABLE*** | | | | |
| 203 | ***AVAILABLE*** | | | | |
| 204 | ***AVAILABLE*** | | | | |
| 205 | ***AVAILABLE*** | | | | |
| 206 | ***AVAILABLE*** | | | | |
| 207 | ***AVAILABLE*** | | | | |
| 208 | ***AVAILABLE*** | | | | |
| 209 | ***AVAILABLE*** | | | | |
| 210 | ***AVAILABLE*** | | | | |
| 211 | ***AVAILABLE*** | | | | |
| 212 | ***AVAILABLE*** | | | | |
| 213 | ***AVAILABLE*** | | | | |
| 214 | ***AVAILABLE*** | | | | |
| 215 | ***AVAILABLE*** | | | | |
| 216 | ***AVAILABLE*** | | | | |
| 217 | ***AVAILABLE*** | | | | |
| 218 | ***AVAILABLE*** | | | | |
| 219 | ***AVAILABLE*** | | | | |
| 220 | ***AVAILABLE*** | | | | |
| 221 | ***AVAILABLE*** | | | | |
| 222 | ***AVAILABLE*** | | | | |
| 223 | ***AVAILABLE*** | | | | |
| 224 | ***AVAILABLE*** | | | | |
| 225 | ***AVAILABLE*** | | | | |
| 226 | ***AVAILABLE*** | | | | |
| 227 | ***AVAILABLE*** | | | | |
| 228 | ***AVAILABLE*** | | | | |
| 229 | ***AVAILABLE*** | | | | |
| 230 | ***AVAILABLE*** | | | | |
| 231 | ***AVAILABLE*** | | | | |
| 232 | ***AVAILABLE*** | | | | |
| 233 | ***AVAILABLE*** | | | | |
| 234 | ***AVAILABLE*** | | | | |
| 235 | ***AVAILABLE*** | | | | |
| 236 | ***AVAILABLE*** | | | | |
| 237 | ***AVAILABLE*** | | | | |
| 238 | ***AVAILABLE*** | | | | |
| 239 | ***AVAILABLE*** | | | | |
| 240 | ***AVAILABLE*** | | | | |
| 241 | ***AVAILABLE*** | | | | |
| 242 | ***AVAILABLE*** | | | | |
| 243 | ***AVAILABLE*** | | | | |
| 244 | ***AVAILABLE*** | | | | |
| 245 | ***AVAILABLE*** | | | | |
| 246 | ***AVAILABLE*** | | | | |
| 247 | ***AVAILABLE*** | | | | |
| 248 | ***AVAILABLE*** | | | | |
| 249 | ***AVAILABLE*** | | | | |
| 250 | ***AVAILABLE*** | | | | |
| 251 | ***AVAILABLE*** | | | | |
| 252 | ***AVAILABLE*** | | | | |
| 253 | ***AVAILABLE*** | | | | |
| 254 | ***AVAILABLE*** | | | | |
| 255 | ***AVAILABLE*** | | | | |