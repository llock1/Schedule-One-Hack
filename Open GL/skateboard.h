#pragma once

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