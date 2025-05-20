#include "pch.h"
#include "features.h"
#include "console.h"
#include "hooks.h"
#include "offsets.h"
#include "player.h"

namespace Cheat {
	namespace Features {
		// bools
		bool GodMode = false;
		bool PoliceDontDoAnything = false;
		bool InfiniteMoney = false;
		bool PoliceDontNoticeDrugDeal = false;
		bool PoliceDontSeeAfterCurfew = false;

		// functions
		void SomeFunction() {
			g_Console->Log(LogType::Success, "Function Activated");
		};

		void Unload() {
			g_Hooks->CleanUp(); 
		}

		void FeatureLoop() {
			while (true) {
				static bool lastGodMode = false;
				static bool lastPoliceDontDoAnything = false;
				static bool lastPoliceDontNoticeDrugDeal = false;
				static bool lastPoliceDontSeeAfterCurfew = false;

				static uintptr_t base = (uintptr_t)GetModuleHandle(L"GameAssembly.dll");

				if (GodMode != lastGodMode) {
					if (GodMode) {
						if (MH_EnableHook(reinterpret_cast<LPVOID*>(base + 0x6F8CA0)) != MH_OK) {
							g_Console->Log(LogType::Error, "Enable Hook Failed");
						}
						else {
							g_Console->Log(LogType::Debug, "GODMODE ACTIVE");
						}
					}
					else {
						if (MH_DisableHook(reinterpret_cast<LPVOID*>(base + 0x6F8CA0)) != MH_OK) {
							g_Console->Log(LogType::Error, "disable Hook Failed");
						}
						else {
							g_Console->Log(LogType::Debug, "GODMODE DEACTIVE");
						}
					}
					lastGodMode = !lastGodMode;
				}

				if (PoliceDontDoAnything != lastPoliceDontDoAnything) {
					if (PoliceDontDoAnything) {
						MH_EnableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToFirstNonLethalAttack));
						MH_EnableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToLethalAttack));
						MH_EnableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToRepeatedNonLethalAttack));
						MH_EnableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToAnnoyingImpact));
						MH_EnableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToAimedAt));
					}
					else {
						MH_DisableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToFirstNonLethalAttack));
						MH_DisableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToLethalAttack));
						MH_DisableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToRepeatedNonLethalAttack));
						MH_DisableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToAnnoyingImpact));
						MH_DisableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToAimedAt));
					}

					lastPoliceDontDoAnything = !lastPoliceDontDoAnything;
				}

				if (InfiniteMoney) {
					if (g_Player->Money < 999999.0f) {
						g_Player->Money = 999999.0f;
					}
				}

				if (PoliceDontNoticeDrugDeal != lastPoliceDontNoticeDrugDeal) {
					if (PoliceDontNoticeDrugDeal) {
						MH_EnableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToDrugDeal));
					}
					else {
						MH_DisableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeRespondToDrugDeal));
					}

					lastPoliceDontNoticeDrugDeal = !lastPoliceDontNoticeDrugDeal;
				}

				if (PoliceDontSeeAfterCurfew != lastPoliceDontSeeAfterCurfew) {
					if (PoliceDontSeeAfterCurfew) {
						MH_EnableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeSeeAfterCurfew));
					}
					else {
						MH_DisableHook(reinterpret_cast<LPVOID*>(base + Offsets::policeSeeAfterCurfew));
					}

					lastPoliceDontSeeAfterCurfew = !lastPoliceDontSeeAfterCurfew;
				}
			}
		}
	}
}