#include "hooks.hpp"

#include "../replay_system/replay_system.hpp"
#include "../gui/gui.hpp"
#include "../replay/replay.hpp"
#include "../practice_fixes/practice_fixes.hpp"

float leftOver = 0.f;

void __fastcall Hooks::CCScheduler_updateH(CCScheduler* self, void*, float dt) {
    const auto playLayer = gd::GameManager::sharedState()->getPlayLayer();
    auto& rs = FembotReplaySystem::getInstance();

    // FPS Bypass
    if (playLayer) {
        const auto fps = rs.getFPS();
        const auto speedhack = self->getTimeScale();

        const float targetDeltaTime = 1.f / (fps * speedhack);

        unsigned times = static_cast<int>((dt + leftOver) / targetDeltaTime);
        for (unsigned i = 0; i < times; i++) {
            if ((rs.frameAdvanceEnabled() && rs.shouldAdvanceFrame()) || !rs.frameAdvanceEnabled()) {
                rs.stopAdvancingFrame();
                CCScheduler_update(self, targetDeltaTime);
            }
        }

        leftOver += dt - targetDeltaTime * times;
    } else {
        CCScheduler_update(self, dt);
    }
}

void __fastcall Hooks::CCKeyboardDispatcher_dispatchKeyboardMSGH(
    CCKeyboardDispatcher* self,
    void*,
    int key,
    bool down
)
{
    // If user is inputting text, don't process key events
    if (ImGui::GetIO().WantCaptureKeyboard) return;

    if (down) {
        if (key == 'G') {
            FembotGUI::getInstance().toggleGUI();
        } else if (key == 'V') {
            FembotReplaySystem::getInstance().toggleFrameAdvance();
        } else if (key == 'C') {
            FembotReplaySystem::getInstance().advanceThisFrame();
        } else {
            CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, down);
        }
    }

}

void __fastcall Hooks::PlayLayer::levelCompleteH(void* self, void*) {
    Hooks::PlayLayer::levelComplete(self);

    auto& pf = PracticeFix::getInstance();

    pf.clearActivatedObjects();
    pf.purgeCheckpoints();
}

void __fastcall Hooks::PlayLayer::resetLevelH(void* self, void*) {
    Hooks::PlayLayer::resetLevel(self);

    gd::PlayLayer* playLayer = gd::GameManager::sharedState()->getPlayLayer();
    auto practice = playLayer->m_isPracticeMode;
    auto& rs = FembotReplaySystem::getInstance();
    auto& pf = PracticeFix::getInstance();

    if (rs.isRecording()) {
        if (practice) {
            if (pf.hasCheckpoints()) {
                auto lastCheckpoint = pf.getLastCheckpoint();
                rs.setFrame(lastCheckpoint.frame);
                std::vector<ReplayAction>& actions = rs.replay.getActions();

                pf.purgeObjectsToSize(
                    lastCheckpoint.activatedObjects_limit_player1, 
                    lastCheckpoint.activatedObjects_limit_player2
                );

                for (auto& object : pf.getActivatedObjects_player1()) {
                    object->m_hasBeenActivated = true;
                }
                for (auto& object : pf.getActivatedObjects_player1()) {
                    object->m_hasBeenActivated = true;
                }

                while (actions.size() != 0 && actions.back().frame >= lastCheckpoint.frame) {
                    actions.pop_back();
                }

                lastCheckpoint.player1.updatePlayer(playLayer->m_player1);
                lastCheckpoint.player2.updatePlayer(playLayer->m_player2);
            }
        } else {
            pf.clearActivatedObjects();
            rs.resetFrame();
            rs.replay.resetActions();
        }
    }
    if (rs.isPlaying()) {
        rs.resetFrame();
    }
}

bool __fastcall Hooks::PlayLayer::initH(CCLayer* self, void*, void* GJGameLevel) {
    auto& rs = FembotReplaySystem::getInstance();
    auto& pf = PracticeFix::getInstance();

    pf.purgeCheckpoints();
    pf.clearActivatedObjects();
    rs.resetFrame();

    if (rs.isRecording()) {
        rs.replay.resetActions();
    } else {
    }
    return Hooks::PlayLayer::init(self, GJGameLevel);
}

int __fastcall Hooks::PlayLayer::createCheckpointH(void* self, void*) {
    PracticeFix::getInstance().createCheckpoint();

    return Hooks::PlayLayer::createCheckpoint(self);
}

int __fastcall Hooks::PlayLayer::removeCheckpointH(void* self, void*) {
    PracticeFix::getInstance().popCheckpoint();

    return Hooks::PlayLayer::removeCheckpoint(self);
}

void __fastcall Hooks::GJBaseGameLayer::pushButtonH(gd::GJBaseGameLayer* self, void*, int button, bool hold) {
    auto& rs = FembotReplaySystem::getInstance();

    if (rs.isPlaying()) return;
    rs.recordAction(hold, button);
    
    pushButton(self, button, hold);
}

void __fastcall Hooks::GJBaseGameLayer::releaseButtonH(gd::GJBaseGameLayer* self, void*, int button, bool hold) {
    auto& rs = FembotReplaySystem::getInstance();

    if (rs.isPlaying()) return;
    rs.recordAction(hold, button);
    
    releaseButton(self, button, hold);
}


void __fastcall Hooks::PlayLayer::updateH(gd::PlayLayer* self, void*, float dt) {
    auto& rs = FembotReplaySystem::getInstance();

    rs.incrementFrame();

    PlayLayer::update(self, dt);
}

/**
 * Initialize the hooks
 */
void Hooks::initialize() {
    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    auto scheduler = reinterpret_cast<uintptr_t>(
        GetProcAddress(GetModuleHandleA("libcocos2d.dll"),
        "?update@CCScheduler@cocos2d@@UAEXM@Z")
    );
    auto dispatcher = reinterpret_cast<uintptr_t>(
        GetProcAddress(GetModuleHandleA("libcocos2d.dll"),
        "?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")
    );

    MH_CreateHook(
        reinterpret_cast<void*>(scheduler),
        reinterpret_cast<void*>(&CCScheduler_updateH),
        reinterpret_cast<void**>(&CCScheduler_update)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(dispatcher),
        reinterpret_cast<void*>(&CCKeyboardDispatcher_dispatchKeyboardMSGH),
        reinterpret_cast<void**>(&CCKeyboardDispatcher_dispatchKeyboardMSG)
    );

    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x2029C0),
        reinterpret_cast<void*>(&PlayLayer::updateH),
        reinterpret_cast<void**>(&PlayLayer::update)
    );
        MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1FD3D0),
        reinterpret_cast<void*>(&Hooks::PlayLayer::levelCompleteH),
        reinterpret_cast<void**>(&Hooks::PlayLayer::levelComplete)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x20BF00),
        reinterpret_cast<void*>(&Hooks::PlayLayer::resetLevelH),
        reinterpret_cast<void**>(&Hooks::PlayLayer::resetLevel)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x20B050),
        reinterpret_cast<void*>(&Hooks::PlayLayer::createCheckpointH),
        reinterpret_cast<void**>(&Hooks::PlayLayer::createCheckpoint)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x20B830),
        reinterpret_cast<void*>(&Hooks::PlayLayer::removeCheckpointH),
        reinterpret_cast<void**>(&Hooks::PlayLayer::removeCheckpoint)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x1FB780),
        reinterpret_cast<void*>(&Hooks::PlayLayer::initH),
        reinterpret_cast<void**>(&Hooks::PlayLayer::init)
    );

    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x111500),
        reinterpret_cast<void*>(&GJBaseGameLayer::pushButtonH),
        reinterpret_cast<void**>(&GJBaseGameLayer::pushButton)
    );
    MH_CreateHook(
        reinterpret_cast<void*>(base + 0x111660),
        reinterpret_cast<void*>(&GJBaseGameLayer::releaseButtonH),
        reinterpret_cast<void**>(&GJBaseGameLayer::releaseButton)
    );
}