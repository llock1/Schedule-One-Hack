#pragma once
#include "pch.h"

namespace Cheat {
	namespace Offsets {
		const uintptr_t playerTakeDamage = 0x6F8CA0;
		const uintptr_t policeRespondToHit = 0x5982D0;  // 5868816
		const uintptr_t policeRespondToFirstNonLethalAttack = 0x5982D0;  // 5868816 (same as above)
		const uintptr_t policeRespondToLethalAttack = 0x5983A8;  // 5869088
		const uintptr_t policeRespondToRepeatedNonLethalAttack = 0x5984A0;  // 5869344
		const uintptr_t policeRespondToAnnoyingImpact = 0x598170;  // 5868384
		const uintptr_t policeRespondToAimedAt = 0x5980C0;  // 5868176
		const uintptr_t policeRespondToDrugDeal = 0x597870;  // 5865728
		const uintptr_t policeSeeAfterCurfew = 0x598580;
	}
}