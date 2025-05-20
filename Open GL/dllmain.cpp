// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "console.h"
#include "hooks.h"
#include "ui.h"
#include "features.h"
#include "player.h"

using namespace Cheat;


int MainThread(HMODULE hModule) {
    g_Hooks = std::make_unique<Hooks>();
    g_UI = std::make_unique<UI>();
    g_Console = std::make_unique<Console>();

    g_Console->InitConsole();
    g_Console->Log(LogType::Success, "Setting up Hook\n");
    
    if (MH_Initialize() != MH_OK) {
        g_Console->Log(LogType::Success, "ERROR");
    }
    else {
        g_Console->Log(LogType::Success, "MinHook Initialized");
    }
    g_Hooks->InitHooks();
    
    Features::FeatureLoop();
    
    g_Hooks->CleanUp();
    FreeLibraryAndExitThread(hModule, 0);

    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    DisableThreadLibraryCalls(hModule);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

