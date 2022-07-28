#include <direct.h>

#include "fembot.hpp"

void Fembot::run() {
    _mkdir(".fembot");
    _mkdir(".fembot\\replays");
    _mkdir(".fembot\\videos");

    MH_Initialize();
    Hooks::initialize();

    MH_EnableHook(MH_ALL_HOOKS);
}