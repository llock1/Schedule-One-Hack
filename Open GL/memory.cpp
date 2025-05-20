#include "pch.h"
#include "memory.h"

namespace Cheat {
	namespace Memory {
		uintptr_t ResolvePointerChain(uintptr_t ptr, std::vector<uintptr_t>& offsets) {
			try {
				uintptr_t addr = ptr;
				for (unsigned int i = 0; i < offsets.size(); ++i)
				{
					if (IsBadReadPtr((uintptr_t*)addr, sizeof(uintptr_t)))
					{
						return NULL; // Invalid address
					}

					addr = *(uintptr_t*)addr;
					addr += offsets[i];
				}

				return addr;
			}
			catch (...) { return NULL; }
		}
	}
}