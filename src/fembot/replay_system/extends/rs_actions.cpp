#pragma once

#include "../replay_system.hpp"

void FembotReplaySystem::recordAction(bool hold, bool player1, bool flip) {
    if (this->isRecording()) {
        auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
        auto isTwoPlayer = playLayer->m_levelSettings->m_twoPlayerMode;
        player1 ^= flip && gd::GameManager::sharedState()->getGameVariable("0010");

        replay.addAction(current_frame, hold, isTwoPlayer && !player1);
    }
}

void FembotReplaySystem::playAction(ReplayAction& action) {
    auto flip = gd::GameManager::sharedState()->getGameVariable("0010");
    if (action.hold) {
        Hooks::GJBaseGameLayer::pushButton(
            gd::GameManager::sharedState()->getPlayLayer(),
            0,
            !action.player2 ^ flip
        );
    } else {
        Hooks::GJBaseGameLayer::releaseButton(
            gd::GameManager::sharedState()->getPlayLayer(),
            0,
            !action.player2 ^ flip
        );
    }
}