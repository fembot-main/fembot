#include "gui.hpp"

#include "../../includes.h"
#include "../replay_system/replay_system.hpp"

/**
 * Main render GUI class
 */
void FembotGUI::render() {
    auto flags = ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar;

    ImGui::Begin("Fembot", nullptr, flags);

    ImGui::InputFloat("FPS", &fps);
    ImGui::SameLine();
    if (ImGui::Button("Set FPS")) {
        if (fps > 0.f) {
            FembotReplaySystem::getInstance().setFPS(fps);
        } else {
            ImGui::OpenPopup("Error");
            FembotReplaySystem::getInstance().setFPS(60.f);
            fps = 60.f;
        }
    }

    ImGui::End();
}

void FembotGUI::init() {
    ImGuiHook::Load([this]() {
        this->render();
    });
}