#include "gui.hpp"

#include "../replay_system/replay_system.hpp"
#include "../replay/replay.hpp"
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

                    auto& gui = FembotGUI::getInstance();
                    auto& rs = FembotReplaySystem::getInstance();
                    
                    ImGui::InputText("Replay Name", gui.replayName, 256);

                    if (ImGui::Button("Load")) {
                        std::string path = ".fembot\\replays";
                        path.append(gui.replayName);
                        path.append(".fem");
                        rs.replay = FembotReplay::loadFromFile(path);
                        std::cout << "replay loaded!";
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Save")) {
                        std::string path(gui.replayName);
                        path = ".fembot\\replays\\" + path + ".fem";
                        rs.replay.saveToFile(path);
                    }

                    ImGui::Separator();
                    
                    if (ImGui::Button("Record")) {
                        rs.setState(ReplaySystemState::RS_RECORDING);
                    }
                    if (ImGui::Button("Playback")) {
                        rs.setState(ReplaySystemState::RS_PLAYING);
                    }

                    // FPS input
                    float& fps = gui.fps;
                    ImGui::InputFloat("FPS", &fps);
                    ImGui::SameLine();
                    if (ImGui::Button("Set FPS")) {
                        if (fps > 0.f) {
                            rs.setFPS(fps);
                        } else {
                            ImGui::OpenPopup("Error");
                            rs.setFPS(60.f);
                            fps = 60.f;
                        }
                    }
                    // End FPS input

                    // Speedhack input
                    float& speed = gui.speed;
                    ImGui::InputFloat("Speedhack", &speed);
                    ImGui::SameLine();
                    if (ImGui::Button("Set Speed")) {
                        if (speed > 0.f) {
                            rs.setSpeed(speed);
                        } else {
                            ImGui::OpenPopup("Error");
                            rs.setSpeed(speed);
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