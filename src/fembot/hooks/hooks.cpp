#include "hooks.hpp"

#include "../replay_system/replay_system.hpp"
#include "../gui/gui.hpp"
#include "../replay/replay.hpp"

float leftOver = 0.f;

void __fastcall Hooks::CCScheduler_updateH(CCScheduler* self, void*, float dt) {
    const auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

    // FPS Bypass
    if (playLayer) {
        const auto fps = FembotReplaySystem::getInstance().getFPS();
        const auto speedhack = self->getTimeScale();

        const float targetDeltaTime = 1.f / (fps * speedhack);

        unsigned times = static_cast<int>((dt + leftOver) / targetDeltaTime);
        for (unsigned i = 0; i < times; i++) {
            CCScheduler_update(self, targetDeltaTime);
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
        } else {
            CCKeyboardDispatcher_dispatchKeyboardMSG(self, key, down);
        }
    }

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
}