#include "pch.h"
#include "UI.h"
#include "features.h"
#include <format>
#include <functional>
#include <string>

namespace Cheat {

    void UI::AddToast(const std::string& msg, float duration = 3.0f) {
        toasts.push_back({ msg, duration});
    }

    void UI::RenderToasts() {
        float y = 10.0f;
        for (auto it = toasts.begin(); it != toasts.end();) {
            ImGui::SetNextWindowPos(ImVec2(10, y), ImGuiCond_Always);
            ImGui::Begin("##Toast", nullptr,
                ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing |
                ImGuiWindowFlags_NoNav);
            ImGui::Text("%s", it->text.c_str());
            ImGui::End();

            it->timer -= ImGui::GetIO().DeltaTime;
            if (it->timer <= 0.0f)
                it = toasts.erase(it);
            else
                ++it;

            y += 40.0f;
        }
    }

    void UI::HandleInput() {
        const auto& items = GetCurrentItems();

        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
            selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
        }

        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
            selectedIndex = (selectedIndex + 1) % items.size();
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
            const MenuEntry& entry = items[selectedIndex];

            if (entry.type == MenuEntryType::Toggle) {
                bool* togglePtr = std::get<bool*>(entry.data);
                *togglePtr = !*togglePtr;
            }
            else if (entry.type == MenuEntryType::Submenu) {
                MenuPage submenu = std::get<MenuPage>(entry.data);
                MenuStack.push_back(submenu);
                currentPage = submenu;
                selectedIndex = 0;
            }
            else if (entry.type == MenuEntryType::Button) {
                std::function<void()> funct = std::get<std::function<void()>>(entry.data);
                funct();
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            int numToIncreaseBy = (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) ? 10 : 1;

            const MenuEntry& entry = items[selectedIndex];

            if (entry.type == MenuEntryType::Int) {
                int* num = std::get<int*>(entry.data);
                *num -= numToIncreaseBy;
            }

            if (entry.type == MenuEntryType::Float) {
                float num = entry.getFloat();
                num -= static_cast<float>(numToIncreaseBy);

                if (num > entry.maxFloat) num = entry.maxFloat;
                if (num < entry.minFloat) num = entry.minFloat;

                entry.setFloat(num);
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
            int numToIncreaseBy = (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) ? 10 : 1;

            const MenuEntry& entry = items[selectedIndex];

            if (entry.type == MenuEntryType::Int) {
                int* num = std::get<int*>(entry.data);
                *num += numToIncreaseBy;
            }

            if (entry.type == MenuEntryType::Float) {
                float num = entry.getFloat();
                num += static_cast<float>(numToIncreaseBy);

                if (num > entry.maxFloat) num = entry.maxFloat;
                if (num < entry.minFloat) num = entry.minFloat;

                entry.setFloat(num);
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Backspace) && currentPage != MenuPage::Main) {
            MenuStack.pop_back();
            currentPage = MenuStack.back();
            selectedIndex = 0;
        }
    }

    void UI::Render() {
        ImGui::SetNextWindowPos(ImVec2(menuX, menuY), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2((float)menuWidth, 0));
        ImGui::SetNextWindowBgAlpha(0.9f);

        ImGui::Begin("SCHEDULE I CHEAT", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize
        );

        float headerHeight = 60.0f;
        ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(45, 45, 45, 255));
        ImGui::BeginChild("Header", ImVec2(0, headerHeight), false,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec2 headerSize = ImGui::GetWindowSize();
        const char* title = "Cheat Name";
        ImVec2 textSize = ImGui::CalcTextSize(title);
        ImGui::SetCursorPosX((headerSize.x - textSize.x) * 0.5f);
        ImGui::SetCursorPosY((headerHeight - textSize.y) * 0.5f);
        ImGui::TextUnformatted(title);

        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "SCHEDULE I: CHEAT");

        std::string positionText = std::format("{} / {}", (selectedIndex + 1), GetCurrentItemCount());
        ImVec2 textSize2 = ImGui::CalcTextSize(positionText.c_str());
        ImGui::SetCursorPosX(menuWidth - textSize2.x - 20);
        ImGui::TextUnformatted(positionText.c_str());

        ImGui::Spacing();
        DrawMenu(GetCurrentItems());
        ImGui::End();
    }

    void UI::DrawMenu(const std::vector<MenuEntry>& items) {
        float width = ImGui::GetContentRegionAvail().x;
        float entryHeight = 32.0f;

        static float animationProgress = 0.0f;
        static int lastSelectedIndex = -1;
        float deltaTime = ImGui::GetIO().DeltaTime;

        if (lastSelectedIndex != selectedIndex) {
            lastSelectedIndex = selectedIndex;
            animationProgress = 0.0f;
        }
        else if (animationProgress < 1.0f) {
            animationProgress += deltaTime * 6.0f;
            if (animationProgress > 1.0f) animationProgress = 1.0f;
        }

        for (int i = 0; i < items.size(); ++i) {
            bool isSelected = (i == selectedIndex);
            const MenuEntry& entry = items[i];

            ImVec4 bgColor = isSelected
                ? ImLerp(ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 0.85f), animationProgress)
                : ImVec4(0, 0, 0, 0);

            ImVec4 textColor = isSelected ? ImVec4(0, 0, 0, 1) : ImVec4(1, 1, 1, 1);

            ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
            ImGui::BeginChild(("entry" + std::to_string(i)).c_str(), ImVec2(width, entryHeight), false);
            ImGui::PopStyleColor();

            ImGui::PushStyleColor(ImGuiCol_Text, textColor);

            std::string leftText = entry.label;
            std::string rightText;

            if (entry.type == MenuEntryType::Toggle) {
                bool* togglePtr = std::get<bool*>(entry.data);
                rightText = (*togglePtr ? "ON" : "OFF");
            }
            else if (entry.type == MenuEntryType::Submenu) {
                rightText = ">";
            }
            else if (entry.type == MenuEntryType::Int) {
                int* num = std::get<int*>(entry.data);
                rightText = std::format("< {} >", *num);
            } else if (entry.type == MenuEntryType::Float) {
                float num = entry.getFloat();
                rightText = std::format("< {} >", num);
            }
            else if (entry.type == MenuEntryType::Info) {
                rightText = "";
            }
            else if (entry.type == MenuEntryType::InfoFloat) {
                float num = entry.getFloat();
                rightText = std::format("{:.2f}", num);
            }

            ImVec2 leftSize = ImGui::CalcTextSize(leftText.c_str());
            ImVec2 rightSize = ImGui::CalcTextSize(rightText.c_str());

            ImGui::SetCursorPosY((entryHeight - leftSize.y) * 0.5f);
            ImGui::SetCursorPosX(10);
            ImGui::TextUnformatted(leftText.c_str());

            ImGui::SetCursorPosY((entryHeight - rightSize.y) * 0.5f);
            ImGui::SetCursorPosX(width - rightSize.x - 10);
            ImGui::TextUnformatted(rightText.c_str());

            ImGui::PopStyleColor();
            ImGui::EndChild();
        }
    }

    int UI::GetCurrentItemCount() const {
        return GetCurrentItems().size();
    }

    const std::vector<MenuEntry>& UI::GetCurrentItems() const {
        switch (currentPage) {
        case MenuPage::Main: return mainItems;
        case MenuPage::Player: return playerItems;
        case MenuPage::Settings: return settingsItems;
        case MenuPage::Police: return policeItems;
        case MenuPage::Skateboard:
            if (g_Player->isSkating && g_Player->skateboard) {
                return skateboardItems;
            }
            return skateboardFailItems;
        }
        static std::vector<MenuEntry> empty;
        return empty;
    }
}
