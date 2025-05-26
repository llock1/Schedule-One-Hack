#pragma once
#include "pch.h"

class PlayerEnergy
{
public:
	char pad_0000[32]; //0x0000
	float currentEnergy; //0x0020
	uint32_t energyDrinksConsumed; //0x0024
	bool debugDisableEnergy; //0x0028
	char pad_0029[3]; //0x0029
	float energyDurationHours; //0x002C
	float energyRechargeTimeHours; //0x0030
	char pad_0034[2067]; //0x0034
}; //Size: 0x0847