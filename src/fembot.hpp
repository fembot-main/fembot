#include <direct.h>
#include <string>
#include <fstream>

#include "includes.h"
#include "hooks.hpp"

class Fembot {
private:

protected:
    Fembot() {}

public:
    static Fembot& getInstance() {
        static Fembot instance;
        return instance;
    }

    Fembot(const Fembot&) = delete;

    void run();
};