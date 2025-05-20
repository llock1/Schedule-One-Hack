#pragma once

class Player
{
public:
	char pad_0000[56]; //0x0000
	float Money; //0x0038
	char pad_003C[236]; //0x003C
	float N00000074; //0x0128
	char pad_012C[788]; //0x012C
}; //Size: 0x0440
inline Player* g_Player = nullptr;