#include "hooks.hpp"

#include "../replay_system/replay_system.hpp"

float leftOver = 0.f;

void __fastcall Hooks::CCScheduler_updateH(CCScheduler* self, void*, float dt) {
    const auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

    if (playLayer) {
        const auto fps = FembotReplaySystem::getInstance().getFPS();
        const auto speedhack = self->getTimeScale();

        const float targetDeltaTime = 1.f / (fps * speedhack);

        unsigned times = static_cast<int>((dt + leftOver) / targetDeltaTime);

        if (dt == 0.f) {
            return CCScheduler_update(self, targetDeltaTime);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned i = 0; i < times; i++) {
            CCScheduler_update(self, targetDeltaTime);
            using namespace std::chrono;

            if (std::chrono::high_resolution_clock::now() - start > 33.333ms) {
                times = i + 1;
                break;
            }
        }
        leftOver += dt - targetDeltaTime * times;
    } else {
        CCScheduler_update(self, dt);
    }
}

/**
 * Initialize the hooks
 */
void Hooks::initialize() {
    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
    auto scheduler = reinterpret_cast<uintptr_t>(GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?update@CCScheduler@cocos2d@@UAEXM@Z"));

    MH_CreateHook(
        reinterpret_cast<void*>(scheduler),
        reinterpret_cast<void*>(&CCScheduler_updateH),
        reinterpret_cast<void**>(&CCScheduler_update)
    );
}