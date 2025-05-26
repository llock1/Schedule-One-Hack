#pragma once
#include "pch.h"

class PlayerCrimeData
{
public:
	char pad_0000[308]; //0x0000
	Vector3 lastKnownPos; //0x0134
	char pad_0140[8]; //0x0140
	float currentArrestProgress; //0x0148
	float currentBodySearchProgress; //0x014C
	float timeSincePursuitStart; //0x0150
	float currentPursuitLevelDuration; //0x0154
	float timeSinceStarted; //0x0158
	char pad_015C[12]; //0x015C
	bool bodySearchPending; //0x0168
	char pad_0169[3]; //0x0169
	float timeSinceLastBodySearch; //0x016C
	bool evadedArrest; //0x0170
	char pad_0171[3]; //0x0171
	float timeSinceLostShot; //0x0174
	char pad_0178[3803]; //0x0178
}; //Size: 0x1053