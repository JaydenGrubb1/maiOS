/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-13
 * @brief // DOC
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cassert>

#include <kernel/arch/x86_64/acpi.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>

/**
 * @brief Root System Description Pointer
 *
 */
struct RSDP {
	char signature[8];
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;
	uint32_t rsdt_addr;
} __attribute__((packed));

/**
 * @brief Extended System Description Pointer
 *
 */
struct XSDP {
	char signature[8];
	uint8_t checksum;
	char oem_id[6];
	uint8_t revision;
	uint32_t rsdt_addr;
	uint32_t length;
	uint64_t xsdt_addr;
	uint8_t extended_checksum;
	unsigned : 24;
} __attribute__((packed));

/**
 * @brief Root System Description Table
 *
 */
struct RSDT {
	ACPI::SDTHeader header;
	uint32_t data[0];
} __attribute__((packed));

static RSDT *rsdt = nullptr;

void ACPI::init(void) {
	Debug::log("Initializing ACPI...");

	Debug::log("Searching for ACPI RSDT...");
	auto mb_xsdp = static_cast<Multiboot2::ACPI_RSDP const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::ACPI_RSDP_2));

	if (mb_xsdp) {
		auto xsdp = reinterpret_cast<XSDP const *>(mb_xsdp->rsdp);
		assert(std::string_view(xsdp->signature, 8) == "RSD PTR ");
		// TODO checksum
		rsdt = reinterpret_cast<RSDT *>(xsdp->xsdt_addr);
	}

	Debug::log_warning("ACPI 2.0 not available, falling back to ACPI 1.0");
	auto mb_rsdp = static_cast<Multiboot2::ACPI_RSDP const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::ACPI_RSDP_1));

	if (mb_rsdp) {
		auto rsdp = reinterpret_cast<RSDP const *>(mb_rsdp->rsdp);
		assert(std::string_view(rsdp->signature, 8) == "RSD PTR ");
		// TODO checksum
		rsdt = reinterpret_cast<RSDT *>(rsdp->rsdt_addr);
	}

	assert(rsdt != nullptr);
	Debug::log_ok("ACPI RSDT found at %p", rsdt);

	Debug::log("Searching for ACPI FADT...");
	auto fadt = static_cast<RSDT const *>(get_entry("FACP"));
	assert(fadt);
	Debug::log_ok("ACPI FADT found at %p", fadt);
	// TODO checksum
	// TODO do something with FADT

	Debug::log_ok("ACPI initialized");
}

void const *ACPI::get_entry(std::string_view signature) {
	assert(rsdt);
	for (size_t i = 0; i < (rsdt->header.length - sizeof(RSDT)) / 4; i++) {
		auto entry = reinterpret_cast<RSDT *>(rsdt->data[i]);
		if (signature == entry->header.signature) {
			return entry;
		}
	}
	return nullptr;
}