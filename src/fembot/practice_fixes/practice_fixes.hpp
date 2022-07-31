#pragma once

#include "../../includes.h"
#include "../hooks/hooks.hpp"
#include "../replay_system/replay_system.hpp"
#include "../../utilities/utilities.hpp"

struct CheckpointPlayer {
    double yAccel;
    float rotation;
    bool hold;
    bool isDashing;
    double jumpAccel;
    CCPoint position;

    bool player;

    void updatePlayer(gd::PlayerObject* player) {
        player->setRotation(rotation);
        setPlayerVar(player, 0x628, yAccel);
        setPlayerVar(player, 0x641, isDashing);
        setPlayerVar(player, 0x520, jumpAccel);
        setPlayerVar(player, 0x67C, position);
        if (hold) {
            Hooks::GJBaseGameLayer::pushButton(gd::GameManager::sharedState()->getPlayLayer(), 0, !player ^ gd::GameManager::sharedState()->getGameVariable("0010"));
        }
    }
};

struct Checkpoint {
    CheckpointPlayer player1;
    CheckpointPlayer player2;

    size_t activatedObjects_limit_player1;
    size_t activatedObjects_limit_player2;

    unsigned frame;
};

class PracticeFix {
protected:
    PracticeFix() {}

    std::vector<Checkpoint> checkpoints;
    std::vector<gd::GameObject*> activatedObjects_player1;
    std::vector<gd::GameObject*> activatedObjects_player2;

public:
    static PracticeFix& getInstance() {
        static PracticeFix instance;
        return instance;
    }

    PracticeFix(const PracticeFix&) = delete;

    void purgeCheckpoints() {
        checkpoints.clear();
    }

    void addCheckpoint(Checkpoint checkpoint) {
        checkpoints.push_back(checkpoint);
    }

    void popCheckpoint() {
        checkpoints.pop_back();
    }

    void createCheckpoint() {
        auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
        auto& rs = FembotReplaySystem::getInstance();

        if (playLayer) {
            Checkpoint checkpoint = {
                {
                    getPlayerVar<double>(playLayer->m_player1, 0x628),
                    playLayer->m_player1->getRotation(),
                    getPlayerVar<bool>(playLayer->m_player1, 0x611) || getPlayerVar<bool>(playLayer->m_player1, 0x612) || getPlayerVar<bool>(playLayer->m_player1, 0x613) || getPlayerVar<bool>(playLayer->m_player1, 0x614),
                    getPlayerVar<bool>(playLayer->m_player1, 0x641),
                    getPlayerVar<double>(playLayer->m_player1, 0x520),
                    getPlayerVar<CCPoint>(playLayer->m_player1, 0x67C),
                    true
                },
                {
                    getPlayerVar<double>(playLayer->m_player2, 0x628),
                    playLayer->m_player2->getRotation(),
                    getPlayerVar<bool>(playLayer->m_player2, 0x611) || getPlayerVar<bool>(playLayer->m_player2, 0x612) || getPlayerVar<bool>(playLayer->m_player2, 0x613) || getPlayerVar<bool>(playLayer->m_player2, 0x614),
                    getPlayerVar<bool>(playLayer->m_player2, 0x641),
                    getPlayerVar<double>(playLayer->m_player2, 0x520),
                    getPlayerVar<CCPoint>(playLayer->m_player2, 0x67C),
                    true
                },
                activatedObjects_player1.size(),
                activatedObjects_player2.size(),
                rs.getFrame()
            };

            this->addCheckpoint(checkpoint);
        }
    }

    Checkpoint getLastCheckpoint() {
        return checkpoints.back();
    }

    void addActivatedObject(gd::GameObject* object, int player) {
        if (player == 1) {
            activatedObjects_player1.push_back(object);
        } else if (player == 2) {
            activatedObjects_player2.push_back(object);
        }
    };

    void applyFix() {
        Checkpoint checkpoint = getLastCheckpoint();

        gd::PlayLayer* playLayer = gd::GameManager::sharedState()->getPlayLayer();

        checkpoint.player1.updatePlayer(playLayer->m_player1);
        checkpoint.player2.updatePlayer(playLayer->m_player2);
    }

    bool hasCheckpoints() {
        return !checkpoints.empty();
    }

    void clearActivatedObjects() {
        activatedObjects_player1.clear();
        activatedObjects_player2.clear();
    }

    void purgeObjectsToSize(size_t size_p1, size_t size_p2) {
        activatedObjects_player1.erase(activatedObjects_player1.begin(), activatedObjects_player1.begin() + size_p1);
        activatedObjects_player2.erase(activatedObjects_player2.begin(), activatedObjects_player2.begin() + size_p2);
    }

    std::vector<gd::GameObject*>& getActivatedObjects_player1() {
        return activatedObjects_player1;
    }

    std::vector<gd::GameObject*>& getActivatedObjects_player2() {
        return activatedObjects_player1;
    }
};