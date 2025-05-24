#pragma once

struct Vector3 {
	float x, y, z;
};

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

class PlayerVisibility
{
public:
	char pad_0000[280]; //0x0000
	float currentVisiblity; //0x0118
	char pad_011C[1828]; //0x011C
}; //Size: 0x0840

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

class Skateboard
{
public:
	char pad_0000[280]; //0x0000
	float currentSpeedKMH; //0x0118
	float currentSteerInput; //0x011C
	float jumpBuildAmount; //0x0120
	char pad_0124[100]; //0x0124
	float turnForce; //0x0188
	float turnChangeRate; //0x018C
	float turnReturnToRestRate; //0x0190
	float turnSpeedBoost; //0x0194
	char pad_0198[8]; //0x0198
	float gravity; //0x01A0
	float brakeForce; //0x01A4
	float reverseTopSpeedKMH; //0x01A8
	char pad_01AC[12]; //0x01AC
	float rotationClampForce; //0x01B8
	bool frictionEnabled; //0x01BC
	char pad_01BD[11]; //0x01BD
	float longitudinalFrictionMultiplier; //0x01C8
	float latitudinalFrictionMultiplier; //0x01CC
	float jumpForce; //0x01D0
	float jumpDurationMin; //0x01D4
	float jumpDurationMax; //0x01D8
	char pad_01DC[28]; //0x01DC
	float jumpForwardBoost; //0x01F8
	float hoverForce; //0x01FC
	float hoverRayLength; //0x0200
	float hoverHeight; //0x0204
	char pad_0208[12]; //0x0208
	float topSpeedKMH; //0x0214
	float pushForceMultiplier; //0x0218
	char pad_021C[12]; //0x021C
	float pushForceDuration; //0x0228
	float pushDelay; //0x022C
	char pad_0230[8]; //0x0230
	bool airMovementEnabled; //0x0238
	char pad_0239[3]; //0x0239
	float airMovementForce; //0x023C
	float airMovementJumpReductionDuration; //0x0240
	char pad_0244[68]; //0x0244
	float jumpForwardForce; //0x0288
	char pad_028C[1468]; //0x028C
}; //Size: 0x0848
