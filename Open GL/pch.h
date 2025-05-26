// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H
#define IMGUI_ENABLE_VIEWPORTS


// add headers that you want to pre-compile here
#include "framework.h"
#include <iostream>
#include <vector>

// MINHOOK
#include "libs/MinHook/MinHook.h"
#pragma comment(lib, "libMinHook.x64.lib")

//#include "libs/IL2CPP_Resolver/IL2CPP_Resolver.hpp"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

// DEAR IMGUI
#include "libs/ImGui/imgui.h"
#include "libs/ImGui/imgui_internal.h"
#include "libs/ImGui/imgui_Impl_dx11.h"
#include "libs/ImGui/imgui_Impl_win32.h"

#include "types.h"
#include "player.h"
#include "player_crime_data.h"
#include "player_energy.h"
#include "skateboard.h"
#include "player_health.h"
#include "player_visibility.h"


#endif //PCH_H
