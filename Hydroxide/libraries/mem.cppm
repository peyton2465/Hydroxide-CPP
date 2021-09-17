module;

#include <Windows.h>
#include <cstdint>

export module mem;

export namespace mem {
	inline std::uintptr_t rebase(std::uintptr_t address, std::uintptr_t base = 0x400000) {
		return address - base + reinterpret_cast<std::uintptr_t>(::GetModuleHandle(nullptr));
	}

	inline std::uintptr_t unbase(std::uintptr_t address, std::uintptr_t base = 0x400000) {
		return (address - reinterpret_cast<std::uintptr_t>(::GetModuleHandle(nullptr))) + base;
	}
}
