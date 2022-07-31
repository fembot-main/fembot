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

        inline bool(__thiscall* init)(CCLayer* self, void* GJGameLevel);
        bool __fastcall initH(CCLayer* self, void*, void* GJGameLevel);

        inline void(__thiscall* levelComplete)(void* self);
        void __fastcall levelCompleteH(void* self, void*);

        inline void(__thiscall* resetLevel)(void* self);
        void __fastcall resetLevelH(void* self, void*);
        
        inline int(__thiscall* createCheckpoint)(void* self);
        int __fastcall createCheckpointH(void* self, void*);
        
        inline int(__thiscall* removeCheckpoint)(void* self);
        int __fastcall removeCheckpointH(void* self, void*);
    }

    namespace GJBaseGameLayer {
        inline void(__thiscall* pushButton)(gd::GJBaseGameLayer* self, int button, bool hold);
        void __fastcall pushButtonH(gd::GJBaseGameLayer* self, void*, int button, bool hold);

        inline void(__thiscall* releaseButton)(gd::GJBaseGameLayer* self, int button, bool hold);
        void __fastcall releaseButtonH(gd::GJBaseGameLayer* self, void*, int button, bool hold);
    }

    void initialize();
}