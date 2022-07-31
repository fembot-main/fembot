#pragma once

#include "../replay/replay.hpp"
#include "../hooks/hooks.hpp"

enum ReplaySystemState {
    RS_RECORDING,
    RS_PLAYING,
};
class FembotReplaySystem {
protected:
    FembotReplaySystem(): fps(60.f), replay(fps), state(RS_RECORDING) {}

    unsigned current_frame;
    float fps;
    FembotReplay replay;

    bool frame_advance;
    bool advance_frame;

    ReplaySystemState state;
    
public:
    static FembotReplaySystem& getInstance() {
        static FembotReplaySystem instance;
        return instance;
    }

    FembotReplaySystem(const FembotReplaySystem&) = delete;

    float getFPS() const { return fps; }
    void setFPS(float fps) {
        this->fps = fps;
        CCDirector::sharedDirector()->setAnimationInterval(1.f / fps);
    }

    void setSpeed(float speed) {
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(speed);
        this->setFPS(this->fps);
    }

    bool isRecording() const { return state == RS_RECORDING; }
    bool isPlaying() const { return state == RS_PLAYING; }
    void setState(ReplaySystemState state) { this->state = state; }

    unsigned getFrame() const { return current_frame; }
    void resetFrame() { current_frame = 0; }
    void incrementFrame() { current_frame++; }

    bool shouldAdvanceFrame();
    bool frameAdvanceEnabled() const;
    void toggleFrameAdvance();
    void advanceThisFrame();
    void stopAdvancingFrame();

    void recordAction(bool hold, bool player1, bool flip = true);
    void playAction(ReplayAction& action);
};