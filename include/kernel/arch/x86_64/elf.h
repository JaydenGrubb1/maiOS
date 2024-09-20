/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief Various structures and constants for the ELF format
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MERGE 0x10
#define SHF_STRINGS 0x20
#define SHF_INFO_LINK 0x40
#define SHF_LINK_ORDER 0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP 0x200
#define SHF_TLS 0x400
#define SHF_MASKOS 0x0ff00000
#define SHF_MASKPROC 0xf0000000

#define ELF64_ST_BIND(i) ((i) >> 4)
#define ELF64_ST_TYPE(i) ((i) & 0xf)
#define ELF64_ST_INFO(b, t) (((b) << 4) + ((t) & 0xf))

namespace ELF {
	/**
	 * @brief ELF section type
	 *
	 */
	enum class SectionType : uint32_t {
		SHT_NULL = 0,
		SHT_PROGBITS = 1,
		SHT_SYMTAB = 2,
		SHT_STRTAB = 3,
		SHT_RELA = 4,
		SHT_HASH = 5,
		SHT_DYNAMIC = 6,
		SHT_NOTE = 7,
		SHT_NOBITS = 8,
		SHT_REL = 9,
		SHT_SHLIB = 10,
		SHT_DYNSYM = 11,
		SHT_INIT_ARRAY = 14,
		SHT_FINI_ARRAY = 15,
		SHT_PREINIT_ARRAY = 16,
		SHT_GROUP = 17,
		SHT_SYMTAB_SHNDX = 18,
		SHT_LOOS = 0x60000000,
		SHT_HIOS = 0x6fffffff,
		SHT_LOPROC = 0x70000000,
		SHT_HIPROC = 0x7fffffff,
		SHT_LOUSER = 0x80000000,
		SHT_HIUSER = 0xffffffff
	};

	/**
	 * @brief ELF section header
	 *
	 */
	struct SectionHeader {
		uint32_t sh_name;
		SectionType sh_type;
		uint64_t sh_flags;
		uint64_t sh_addr;
		uint64_t sh_offset;
		uint64_t sh_size;
		uint32_t sh_link;
		uint32_t sh_info;
		uint64_t sh_addralign;
		uint64_t sh_entsize;
	};

	/**
	 * @brief ELF symbol type
	 *
	 */
	enum class SymbolType {
		STT_NOTYPE = 0,
		STT_OBJECT = 1,
		STT_FUNC = 2,
		STT_SECTION = 3,
		STT_FILE = 4,
		STT_COMMON = 5,
		STT_TLS = 6,
		STT_LOOS = 10,
		STT_HIOS = 12,
		STT_LOPROC = 13,
		STT_HIPROC = 15
	};

	/**
	 * @brief ELF symbol table entry
	 *
	 */
	struct SymbolTableEntry {
		uint32_t st_name;
		uint8_t st_info;
		uint8_t st_other;
		uint16_t st_shndx;
		uint64_t st_value;
		uint64_t st_size;
	};
}