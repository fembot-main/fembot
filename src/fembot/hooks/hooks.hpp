#pragma once

#include "../../includes.h"

namespace Hooks {
    inline void(__thiscall* CCScheduler_update)(CCScheduler* self, float dt);
    void __fastcall CCScheduler_updateH(CCScheduler* self, void*, float dt);

    inline void(__thiscall* CCKeyboardDispatcher_dispatchKeyboardMSG)(CCKeyboardDispatcher* self, int key, bool down);
    void __fastcall CCKeyboardDispatcher_dispatchKeyboardMSGH(CCKeyboardDispatcher* self, void*, int key, bool down);

    namespace PlayLayer {
        inline void(__thiscall* update)(gd::PlayLayer* self, float dt);
        void __fastcall updateH(gd::PlayLayer* self, void*, float dt);
    }

    namespace GJBaseGameLayer {
        inline void(__thiscall* pushButton)(gd::GJBaseGameLayer* self, int button, bool hold);
        void __fastcall pushButtonH(gd::GJBaseGameLayer* self, void*, int button, bool hold);

        inline void(__thiscall* releaseButton)(gd::GJBaseGameLayer* self, int button, bool hold);
        void __fastcall releaseButtonH(gd::GJBaseGameLayer* self, void*, int button, bool hold);
    }

    void initialize();
}