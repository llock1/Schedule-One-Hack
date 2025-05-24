#pragma once
#include "pch.h"
#include <memory>
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>

namespace Cheat {

    class Hooks {
    public:
        uintptr_t gameAssemblyBase;
        uintptr_t baseAddress;

        HMODULE hModule;
        void InitHooks();
        void CleanUp();

        //{
//    "Address": 5865728,
//        "Name" : "ScheduleOne.Police.NPCResponses_Police$$NoticedDrugDeal",
//        "Signature" : "void ScheduleOne_Police_NPCResponses_Police__NoticedDrugDeal (ScheduleOne_Police_NPCResponses_Police_o* __this, ScheduleOne_PlayerScripts_Player_o* player, const MethodInfo* method);",
//        "TypeSignature" : "viii"
//    }

        //{
//    "Address": 7015952,
//        "Name" : "ScheduleOne.PlayerScripts.Player$$set_IsArrested",
//        "Signature" : "void ScheduleOne_PlayerScripts_Player__set_IsArrested (ScheduleOne_PlayerScripts_Player_o* __this, bool value, const MethodInfo* method);",
//        "TypeSignature" : "viii"
//    },


       static void __stdcall take_player_damage_hook(DWORD* __this, float damage, bool flinch, bool playBloodMist, DWORD* method);
       static void __stdcall police_respond_to_lethal_attack_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method);
       static void __stdcall police_respond_to_first_nonlethal_attack_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method);
       static void __stdcall police_respond_to_repeated_nonlethal_attack_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method);
       static void __stdcall police_respond_to_annoying_impact_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method);
       static void __stdcall police_respond_to_aimed_at_hook(DWORD* __this, DWORD* player, DWORD* method);
       static void __stdcall police_notice_drug_deal_hook(DWORD* __this, DWORD* player, DWORD* method);
       static void __stdcall police_see_after_curfew_hook(DWORD* __this, DWORD* player, DWORD* method);
       static void __stdcall is_arrested_hook(DWORD* __this, bool value, DWORD* method);

    };

    // Hooked function declaration
    HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

    // Globals (only declared here, initialized in cpp)
    extern ID3D11Device* g_pd3dDevice;
    extern ID3D11DeviceContext* g_pd3dDeviceContext;
    extern ID3D11RenderTargetView* g_mainRenderTargetView;
    extern HWND g_hWnd;
    extern WNDPROC oWndProc;

    // Original Present function pointer (to call original Present)
    extern decltype(&hkPresent) oPresent;

    inline std::unique_ptr<Hooks> g_Hooks;
}
