/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief Retrieves debug symbols for the kernel
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <lib/libc++/pair.h>

#include <kernel/arch/x86_64/elf.h>
#include <kernel/arch/x86_64/ksyms.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>

static ELF::SectionHeader const *symtab = nullptr;
static char *strtab = nullptr;

kstd::pair<const char *, uintptr_t> KSyms::get_symbol(void *addr) {
	if (!is_available()) {
		return {nullptr, 0};
	}

	for (size_t i = 0; i < symtab->sh_size / sizeof(ELF::SymbolTableEntry); i++) {
		auto sym = reinterpret_cast<ELF::SymbolTableEntry *>(symtab->sh_addr)[i];

		if (ELF64_ST_TYPE(sym.st_info) == static_cast<uint8_t>(ELF::SymbolType::STT_FUNC) && sym.st_size != 0) {
			if (addr >= reinterpret_cast<void *>(sym.st_value) && addr < reinterpret_cast<void *>(sym.st_value + sym.st_size)) {
				return {&strtab[sym.st_name], sym.st_value};
			}
		}
	}

	return {nullptr, 0};
}

void KSyms::init(void) {
	Debug::log("Initializing kernel symbol table...");

	auto elf = reinterpret_cast<Multiboot2::ELFSymbols const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::ELF_SYMBOLS));
	if (elf == nullptr) {
		Debug::log_failure("No ELF symbols found in multiboot2 info block");
		return;
	}

	Debug::log("Searching for ELF debug sections...");
	auto sections = reinterpret_cast<ELF::SectionHeader const *>(elf->sections);

	for (size_t i = 0; i < elf->entry_num; i++) {
		if (sections[i].sh_type == ELF::SectionType::SHT_SYMTAB) {
			Debug::log_ok("Found SYMTAB section");
			symtab = &sections[i];
			break;
		}
	}

	if (symtab == nullptr) {
		Debug::log_failure("No SYMTAB section found");
		return;
	}
	if (symtab->sh_link >= elf->entry_num) {
		Debug::log_failure("SYMTAB section has invalid link");
		return;
	}
	if (sections[symtab->sh_link].sh_type != ELF::SectionType::SHT_STRTAB) {
		Debug::log_failure("SYMTAB section has invalid link");
		return;
	}

	Debug::log_ok("Found STRTAB section");
	strtab = reinterpret_cast<char *>(sections[symtab->sh_link].sh_addr);

	Debug::log_ok("Kernel symbol table initialized");
}

bool KSyms::is_available(void) {
	return strtab != nullptr;
}