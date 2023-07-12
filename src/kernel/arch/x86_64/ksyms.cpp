/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/elf.h>
#include <kernel/arch/x86_64/ksyms.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static ELF::SectionHeader *symtab = nullptr;
static char *strtab = nullptr;

const char *KSyms::get_symbol(void *addr, uint64_t *sym_addr) {
	if (!is_available()) {
		return nullptr;
	}

	for (size_t i = 0; i < symtab->sh_size / sizeof(ELF::SymbolTableEntry); i++) {
		auto sym = ((ELF::SymbolTableEntry *)symtab->sh_addr)[i];

		if (ELF64_ST_TYPE(sym.st_info) == ELF::SymbolType::STT_FUNC && sym.st_size != 0) {
			if (addr >= (void *)sym.st_value && addr < (void *)(sym.st_value + sym.st_size)) {
				*sym_addr = sym.st_value;
				return &strtab[sym.st_name];
			}
		}
	}

	return nullptr;
}

void KSyms::init(void) {
	Debug::log("Initializing kernel symbol table...");

	auto elf = (Multiboot2::ELFSymbols *)Multiboot2::get_entry(Multiboot2::ELF_SYMBOLS);
	if (elf == nullptr) {
		Debug::log_failure("No ELF symbols found in multiboot2 info block");
		return;
	}

	Debug::log("Searching for SYMTAB section...");
	auto sections = (ELF::SectionHeader *)((char *)elf->sections + 4);

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
	strtab = (char *)sections[symtab->sh_link].sh_addr;

	Debug::log_ok("Kernel symbol table initialized");
}

bool KSyms::is_available(void) {
	return strtab != nullptr;
}