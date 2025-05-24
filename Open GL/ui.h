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

        MenuEntry(std::string label, float* num, float maxNum, float minNum)
            : label(std::move(label)), type(MenuEntryType::Float), data(num), maxFloat(maxNum), minFloat(minNum) {
        }
    };



    class UI {
    public:
        int menuWidth = 420;
        int menuY = 30;
        int menuX = 30;
        float example = 0.0f;

        MenuPage currentPage = MenuPage::Main;

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
            MenuEntry("Gravity", &example, 100.0f, -100.0f),
        };

        std::vector<MenuEntry> playerItems = {
            MenuEntry("Godmode", &Features::GodMode),
            MenuEntry("Gravity", &example, 100.0f, -100.0f),
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
