#pragma once
#include "pch.h"

namespace Cheat {
	namespace Memory {
		uintptr_t ResolvePointerChain(uintptr_t baseAddress, std::vector<uintptr_t>& offsets);
	}
}