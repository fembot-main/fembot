#include <direct.h>

#include "fembot.hpp"
#include "gui/gui.hpp"

/**
 * Main run function for the program
 */
void Fembot::run() {
    // Create folders for the bot
    _mkdir(".fembot");
    _mkdir(".fembot\\replays");
    _mkdir(".fembot\\videos");

    // Initialize the hooks
    MH_Initialize();
    Hooks::initialize();

    MH_EnableHook(MH_ALL_HOOKS);
}
