#include <direct.h>
#include <string>
#include <fstream>

#include "includes.h"
#include "fembot/fembot.hpp"
#include "fembot/gui/gui.hpp"

#define _CONSOLE

DWORD WINAPI thread_func(void* hModule) {
    #ifdef _CONSOLE
    AllocConsole();
    std::ofstream conout("CONOUT$", std::ios::out);
    std::ifstream conin("CONIN$", std::ios::in);
    std::cout.rdbuf(conout.rdbuf());
    std::cin.rdbuf(conin.rdbuf());
    #endif
    std::cout << "Fembot injected!" << std::endl;

    // Start the bot
    Fembot::getInstance().run();

    #ifdef _CONSOLE
    std::getline(std::cin, std::string());

    MH_Uninitialize();

    conout.close();
    conin.close();
    FreeConsole();
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    #endif

    return 0;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        FembotGUI::getInstance().init();
        auto h = CreateThread(0, 0, thread_func, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}