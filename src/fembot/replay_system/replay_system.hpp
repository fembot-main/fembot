#pragma once

#include "../replay/replay.hpp"

class FembotReplaySystem {
protected:
    FembotReplaySystem(): fps(60.f), replay(fps) {}

    float fps;
    FembotReplay replay;
public:
    static FembotReplaySystem& getInstance() {
        static FembotReplaySystem instance;
        return instance;
    }

    FembotReplaySystem(const FembotReplaySystem&) = delete;

    float getFPS() const {
        return fps;
    }

    void setFPS(float fps) {
        this->fps = fps;
    }
};