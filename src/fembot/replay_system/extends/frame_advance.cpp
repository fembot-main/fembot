#pragma once

#include "../replay_system.hpp"

bool FembotReplaySystem::shouldAdvanceFrame() {
    return advance_frame;
}

bool FembotReplaySystem::frameAdvanceEnabled() const {
    return frame_advance;
}

void FembotReplaySystem::toggleFrameAdvance() {
    frame_advance = !frame_advance;
}

void FembotReplaySystem::advanceThisFrame() {
    advance_frame = true;
}

void FembotReplaySystem::stopAdvancingFrame() {
    advance_frame = false;
}