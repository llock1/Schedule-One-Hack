#pragma once

class PlayerHealth
{
public:
	char pad_0000[280]; //0x0000
	bool isAlive; //0x0118
	char pad_0119[3]; //0x0119
	float currentHealth; //0x011C
	float timeSinceLastDamage; //0x0120
	char pad_0124[3900]; //0x0124
}; //Size: 0x1060