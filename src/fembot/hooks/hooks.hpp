#pragma once

#include "../../includes.h"

namespace Hooks {
    inline void(__thiscall* CCScheduler_update)(CCScheduler* self, float dt);
    void __fastcall CCScheduler_updateH(CCScheduler* self, void*, float dt);

    inline void(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(CCKeyboardDispatcher* self, int key, bool down);
    void __fastcall CCKeyboardDispatcher_dispatchKeyboardMSGH(CCKeyboardDispatcher* self, void*, int key, bool down);

    void initialize();
}