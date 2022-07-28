#include "../../includes.h"

namespace Hooks {
    inline void(__thiscall* CCScheduler_update)(CCScheduler* self, float dt);
    void __fastcall CCScheduler_updateH(CCScheduler* self, void*, float dt);

    void initialize();
}