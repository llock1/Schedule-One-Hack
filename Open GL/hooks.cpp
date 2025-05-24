#include "pch.h"
#include "hooks.h"
#include "console.h"
#include "ui.h"
#include "offsets.h"
#include "memory.h"
#include "player.h"
#include "features.h"
#include <string>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void(__fastcall* take_player_damage_o)(DWORD*, float, bool, bool, const DWORD*);
void(__fastcall* police_respond_to_first_nonlethal_attack_o)(DWORD*, DWORD*, DWORD*, DWORD*);
void(__fastcall* police_respond_to_lethal_attack_o)(DWORD*, DWORD*, DWORD*, DWORD*);
void(__fastcall* police_respond_to_repeated_nonlethal_attack_o)(DWORD*, DWORD*, DWORD*, DWORD*);
void(__fastcall* police_respond_to_annoying_impact_o)(DWORD*, DWORD*, DWORD*, DWORD*);
void(__fastcall* police_respond_to_aimed_at_o)(DWORD*, DWORD*, DWORD*);
void(__fastcall* police_notice_drug_deal_o)(DWORD*, DWORD*, DWORD*);
void(__fastcall* police_see_after_curfew_o)(DWORD*, DWORD*, DWORD*);
void(__fastcall* is_arrested_o)(DWORD*, bool, DWORD*);


namespace Cheat {

    // Global variable initialization (only done here)
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    HWND g_hWnd = nullptr;
    WNDPROC oWndProc = nullptr;
    decltype(&hkPresent) oPresent = nullptr;

    void CreateRenderTarget(IDXGISwapChain* pSwapChain) {
        ID3D11Texture2D* pBackBuffer = nullptr;
        if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer))) {
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
            pBackBuffer->Release();
        }
    }

    void CleanupRenderTarget() {
        if (g_mainRenderTargetView) {
            g_mainRenderTargetView->Release();
            g_mainRenderTargetView = nullptr;
        }
    }

    LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (ImGui::GetCurrentContext() && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;

        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
        static bool initialized = false;

        if (!initialized) {
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice))) {
                g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);

                DXGI_SWAP_CHAIN_DESC sd;
                pSwapChain->GetDesc(&sd);
                g_hWnd = sd.OutputWindow;
                 
                CreateRenderTarget(pSwapChain);

                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO();
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

                io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", 18.0f);

                ImGui_ImplWin32_Init(g_hWnd);
                ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

                oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);

                initialized = true;
            }
        }

        // Start ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        g_UI->HandleInput();
        g_UI->Render();

        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        return oPresent(pSwapChain, SyncInterval, Flags);
    }

    void Hooks::InitHooks() {



        gameAssemblyBase = (uintptr_t)GetModuleHandle(L"GameAssembly.dll");
        uintptr_t basePtr = gameAssemblyBase + 0x03B08DB0;

        std::vector<uintptr_t> offsets = { 0x40, 0xC8, 0x28, 0x48, 0xB8, 0x48, 0x0 };
        uintptr_t playerAddress = Memory::ResolvePointerChain(basePtr, offsets);

        while (playerAddress == NULL) {
            Sleep(100);
            playerAddress = Memory::ResolvePointerChain(basePtr, offsets);
        }


        g_Player = reinterpret_cast<Player*>(playerAddress);
        if (!playerAddress) {
            std::cout << "Pointer chain failed (null address)" << std::endl;
            return;
        }
        printf("Local Player Address: 0x%p\n", g_Player);

        std::string floatStr = std::to_string(g_Player->playerHealth->currentHealth);
        std::string skateStr = std::to_string(g_Player->isSkating);

        g_Console->Log(LogType::Debug, floatStr.c_str());
        g_Console->Log(LogType::Debug, skateStr.c_str());

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = GetForegroundWindow();
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL featureLevel;
        ID3D11Device* pDevice = nullptr;
        ID3D11DeviceContext* pContext = nullptr;
        IDXGISwapChain* pSwapChain = nullptr;

        if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
            D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pContext)))
        {
            void** vTable = *reinterpret_cast<void***>(pSwapChain);
            oPresent = (decltype(&hkPresent))vTable[8];

            if (MH_CreateHook(vTable[8], &hkPresent, reinterpret_cast<void**>(&oPresent)) == MH_OK &&
                MH_EnableHook(vTable[8]) == MH_OK)
            {
                g_Console->Log(LogType::Success, "Successfully hooked IDXGISwapChain::Present");
            }
            else {
                g_Console->Log(LogType::Error, "Failed to hook Present");
            }

            pSwapChain->Release();
            pDevice->Release();
            pContext->Release();
        }
        else {
            g_Console->Log(LogType::Error, "Failed to create dummy D3D11 device for vtable extraction");
        }

        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::playerTakeDamage), &take_player_damage_hook, (LPVOID*)&take_player_damage_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook player damage");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked Player damage");
        }

        // Assuming these hooks are set up somewhere in your Hooks.cpp or similar
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::policeRespondToFirstNonLethalAttack),
            &police_respond_to_first_nonlethal_attack_hook, (LPVOID*)&police_respond_to_first_nonlethal_attack_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook police responding to first non-lethal attack");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked police response to first non-lethal attack");
        }

        // Hook for Responding to Lethal Attack
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::policeRespondToLethalAttack),
            &police_respond_to_lethal_attack_hook, (LPVOID*)&police_respond_to_lethal_attack_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook police responding to lethal attack");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked police response to lethal attack");
        }

        // Hook for Responding to Repeated Non-Lethal Attack
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::policeRespondToRepeatedNonLethalAttack),
            &police_respond_to_repeated_nonlethal_attack_hook, (LPVOID*)&police_respond_to_repeated_nonlethal_attack_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook police responding to repeated non-lethal attack");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked police response to repeated non-lethal attack");
        }

        // Hook for Responding to Annoying Impact
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::policeRespondToAnnoyingImpact),
            &police_respond_to_annoying_impact_hook, (LPVOID*)&police_respond_to_annoying_impact_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook police responding to annoying impact");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked police response to annoying impact");
        }

        // Hook for Responding to Being Aimed At
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::policeRespondToAimedAt),
            &police_respond_to_aimed_at_hook, (LPVOID*)&police_respond_to_aimed_at_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook police responding to being aimed at");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked police response to being aimed at");
        }


        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::policeRespondToDrugDeal),
            &police_notice_drug_deal_hook, (LPVOID*)&police_notice_drug_deal_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook police noticing drug deal");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked police noticing drug deal");
        }
        
        
        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::policeSeeAfterCurfew),
            &police_see_after_curfew_hook, (LPVOID*)&police_see_after_curfew_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook police seeing after curew");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked police seeing after curfew");
        }

        if (MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::isArrested),
            &is_arrested_hook, (LPVOID*)&is_arrested_o) != MH_OK) {
            g_Console->Log(LogType::Error, "failed to hook is arrested");
        }
        else {
            g_Console->Log(LogType::Success, "Hooked is arrested");
        }

        MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssemblyBase + Offsets::isArrested));

    }

    void __stdcall Hooks::take_player_damage_hook(DWORD* __this, float damage, bool flinch, bool playBloodMist, DWORD* method) {
        damage = 0.0;
        flinch = false;
        playBloodMist = false;

        return take_player_damage_o(__this, damage, flinch, playBloodMist, method);
    }

    void __stdcall Hooks::police_respond_to_lethal_attack_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method) {
        return;
    }
    void __stdcall Hooks::police_respond_to_first_nonlethal_attack_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method) {
        return;
    }

    void __stdcall Hooks::police_respond_to_repeated_nonlethal_attack_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method) {
        return;
    }

    void __stdcall Hooks::police_respond_to_annoying_impact_hook(DWORD* __this, DWORD* perpetrator, DWORD* impact, DWORD* method) {
        return;
    }

    void __stdcall Hooks::police_respond_to_aimed_at_hook(DWORD* __this, DWORD* player, DWORD* method) {
        return;
    }

    void __stdcall Hooks::police_notice_drug_deal_hook(DWORD* __this, DWORD* player, DWORD* method) {
        return;
    }

    void __stdcall Hooks::police_see_after_curfew_hook(DWORD* __this, DWORD* player, DWORD* method) {
        return;
    }

    void __stdcall Hooks::is_arrested_hook(DWORD* __this, bool value, DWORD* method) {

        if (&Features::NeverWanted) {
            g_Console->Log(LogType::Debug, "NOT ARRESTED");
            value = false;
        }

        g_Console->Log(LogType::Debug, "ARRESTED");

        return is_arrested_o(__this, value, method);
    }


    void Hooks::CleanUp() {
        if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK) {
            g_Console->Log(LogType::Error, "Failed to unhook.");
        }
        else {
            g_Console->Log(LogType::Success, "Successfully unhooked");
        }
        if (MH_Uninitialize() != MH_OK) {
            g_Console->Log(LogType::Error, "Failed to unint");
        }
        else {
            g_Console->Log(LogType::Success, "Successfully uninitialized MinHook");
        }
    }
}
