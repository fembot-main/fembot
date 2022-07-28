#include <direct.h>
#include <string>
#include <fstream>

#include "../includes.h"
#include "hooks/hooks.hpp"

class Fembot {
protected:
    Fembot() {}

public:
    /**
     * Get a singleton instance of the bot
     *
     * @returns Reference to the bot's instance
     */
    static Fembot& getInstance() {
        static Fembot instance;
        return instance;
    }

    Fembot(const Fembot&) = delete;

    void run();
};