#include "gui.hpp"

#include "../replay_system/replay_system.hpp"

/**
 * Main render GUI class
 */
void renderGUI() {
    if (FembotGUI::getInstance().shouldRender()) {
        auto flags = ImGuiWindowFlags_HorizontalScrollbar
            | ImGuiWindowFlags_AlwaysHorizontalScrollbar;

        if (ImGui::Begin("Fembot", nullptr, flags)) {
            if (ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_None)) {
                if (ImGui::BeginTabItem("Record")) {

                    // FPS input
                    float& fps = FembotGUI::getInstance().fps;
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
                    // End FPS input

                    // Speedhack input
                    float& speed = FembotGUI::getInstance().speed;
                    ImGui::InputFloat("Speedhack", &speed);
                    ImGui::SameLine();
                    if (ImGui::Button("Set Speed")) {
                        if (speed > 0.f) {
                            FembotReplaySystem::getInstance().setSpeed(speed);
                        } else {
                            ImGui::OpenPopup("Error");
                            FembotReplaySystem::getInstance().setSpeed(speed);
                            speed = 1.f;
                        }
                    }
                    // End speedhack input

                    ImGui::EndTabItem();
                }
            }
        }

        ImGui::End();
    }
}