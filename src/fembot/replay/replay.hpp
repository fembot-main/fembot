#pragma once

#include <vector>
#include <algorithm>
#include <string>

struct ReplayAction {
    unsigned int frame;
    bool hold;
    bool player2;
};

class FembotReplay {
protected:
    std::vector<ReplayAction> actions;
    float fps;

public:
    FembotReplay(float fps) : fps(fps) {}

    /**
     * Get the FPS of the replay
     *
     * @returns The FPS of the replay
     */
    float getFPS() const {
        return fps;
    }

    void setFPS(float new_fps) {
        fps = new_fps;
    }

    /**
     * @brief Add an action to the replay
     *
     * @param frame The frame of the action
     * @param hold Whether the action is a click or a release (true = click, false = release)
     * @param player2 Whether the action is for player 2 (true = player 2, false = player 1)
     */
    void addAction(unsigned int frame, bool hold, bool player2) {
        actions.push_back({frame, hold, player2});
    }

    /**
     * Get the actions of the replay
     *
     * @returns The actions of the replay
     */
    auto& getActions() {
        return actions;
    }

    void resetActions() {
        actions.clear();
    }

    void setAllActions(std::vector<ReplayAction> newActions) {
        actions = newActions;
    }

    /**
     * @brief Remove actions after a specific frame
     *
     * @param frame The frame to remove actions after
     */
    void removeActionsAfter(unsigned int frame) {
        auto it = std::find_if(actions.begin(), actions.end(), [frame](const ReplayAction& action) {
            return action.frame >= frame;
        });
        actions.erase(it, actions.end());
    }

    /**
     * @brief Save the replay to a binary .fem file
     *
     * @param filename The filename to save the replay to
     */
    void saveToFile(const std::string filename);

    /**
     * @brief Load a replay from a binary .fem file
     *
     * @param filename The filename to load the replay from
     *
     * @returns The loaded replay
     */
    static FembotReplay loadFromFile(const std::string& filename);
};