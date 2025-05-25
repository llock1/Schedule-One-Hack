#pragma once
#include "pch.h"
#include <vector>
#include <variant>
#include <functional>
#include "features.h"
#include "player.h"

namespace Cheat {

    enum class MenuEntryType {
        Toggle,
        Submenu,
        Button,
        Int,
        Float,
        FloatButton,
    };

    enum class MenuPage {
        Main,
        Player,
        Skateboard,
        Graphics,
        Movement,
        Settings,
        Police,
    };

    struct MenuEntry {
        std::string label;
        MenuEntryType type;
        std::variant<bool*, MenuPage, std::function<void()>, int*, float*> data;
        int maxInt;
        int minInt;

        float maxFloat;
        float minFloat;

        std::function<float()> getFloat;
        std::function<void(float)> setFloat;

        MenuEntry(std::string label, bool* togglePtr)
            : label(std::move(label)), type(MenuEntryType::Toggle), data(togglePtr) {
        }

        MenuEntry(std::string label, MenuPage submenuTarget)
            : label(std::move(label)), type(MenuEntryType::Submenu), data(submenuTarget) {
        }

        MenuEntry(std::string label, std::function<void()> func)
            : label(std::move(label)), type(MenuEntryType::Button), data(func) {
        }

        MenuEntry(std::string label, int* num, int maxNum, int minNum)
            : label(std::move(label)), type(MenuEntryType::Int), data(num), maxInt(maxNum), minInt(minNum) {
        }

        MenuEntry(std::string label, std::function<float()> setNum, std::function<void(float)> setFloat, float maxNum, float minNum)
            : label(std::move(label)), type(MenuEntryType::Float), getFloat(setNum), setFloat(setFloat), maxFloat(maxNum), minFloat(minNum) {
        }
    };

    struct SimpleToast {
        std::string text;
        float timer;   // Current remaining time
        float maxTime; // Total lifetime
    };

    


    class UI {
    public:
        int menuWidth = 420;
        int menuY = 30;
        int menuX = 30;
        float example = 0.0f;

        MenuPage currentPage = MenuPage::Main;

        std::vector<SimpleToast> toasts;
        void RenderToasts();
        void AddToast(const std::string& msg, float duration);


        std::vector<MenuPage> MenuStack = {
            MenuPage::Main,
        };

        int selectedIndex = 0;

        std::vector<MenuEntry> mainItems = {
            MenuEntry("Player", MenuPage::Player),
            MenuEntry("Skateboard", MenuPage::Skateboard),
            MenuEntry("Settings", MenuPage::Settings),
            MenuEntry("Exit", Features::Unload)
        };
         
        std::vector<MenuEntry> skateboardItems = {
            MenuEntry("Gravity",
                []() -> float {
                    return g_Player->isSkating ? g_Player->skateboard->gravity : 0.0f;
                },
                [](float val) {
                    if (g_Player->isSkating && g_Player->skateboard) {
                        g_Player->skateboard->gravity = val;
                    }
                },
                100.0f, -100.0f
            ),

            MenuEntry("Turn Force",
                []() -> float {
                    return g_Player->isSkating && g_Player->skateboard ? g_Player->skateboard->turnForce : 0.0f;
                },
                [](float num) {
                    if (g_Player->isSkating && g_Player->skateboard) {
                        g_Player->skateboard->turnForce = num;
                    }
                },
                100.0f, -100.0f
            )
        };
        
        std::vector<MenuEntry> playerItems = {
            MenuEntry("Godmode", &Features::GodMode),
            MenuEntry("Police Stuff", MenuPage::Police),
            MenuEntry("Infinite Money", &Features::InfiniteMoney),
            MenuEntry("Function Example", Features::SomeFunction)
        };

        std::vector<MenuEntry> policeItems = {
            MenuEntry("Police Dont Do Anything", &Features::PoliceDontDoAnything),
            MenuEntry("Police Dont Notice Drug Deal", &Features::PoliceDontNoticeDrugDeal),
            MenuEntry("Police Dont Care About Curfew", &Features::PoliceDontSeeAfterCurfew),
            MenuEntry("Never Wanted", &Features::NeverWanted),
        };

        std::vector<MenuEntry> settingsItems = {
            MenuEntry("Menu Width", &menuWidth, 1000, 300),
            MenuEntry("Menu X", &menuX, 2000, 0),
            MenuEntry("Menu Y", &menuY, 2000, 0),
        };

        void HandleInput();
        void Render();
    private:
        void DrawMenu(const std::vector<MenuEntry>& items);
        int GetCurrentItemCount() const;
        const std::vector<MenuEntry>& GetCurrentItems() const;
    };
    inline std::unique_ptr<UI> g_UI;
}
