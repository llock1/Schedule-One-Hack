#pragma once

// Created with ReClass.NET 1.2 by KN4CK3R

class Player
{
public:
	char pad_0000[320]; //0x0000
	Vector3 eyePos; //0x0140
	char pad_014C[20]; //0x014C
	class PlayerVisibility* playerVisibility; //0x0160
	char pad_0168[16]; //0x0168
	class PlayerHealth* playerHealth; //0x0178
	class PlayerCrimeData* playerCrimeData; //0x0180
	char pad_0188[160]; //0x0188
	float timeSinceVehicleExit; //0x0228
	bool isCrouched; //0x022C
	char pad_022D[11]; //0x022D
	bool isReadyToSleep; //0x0238
	bool isSkating; //0x0239
	char pad_023A[6]; //0x023A
	class Skateboard* skateboard; //0x0240
	char pad_0248[16]; //0x0248
	bool isSleeping; //0x0258
	bool isRagdolled; //0x0259
	bool isArrested; //0x025A
	bool isTased; //0x025B
	bool isUnconscious; //0x025C
	char pad_025D[35]; //0x025D
	bool hasCompletedIntro; //0x0280
	char pad_0281[3]; //0x0281
	Vector3 cameraPos; //0x0284
	char pad_0290[3498]; //0x0290
}; //Size: 0x103A
inline Player* g_Player = nullptr;

