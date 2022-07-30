#pragma once

#include "../../includes.h"

class FembotGUI {
protected:
    FembotGUI() {}
    bool show_gui = true;
public:
    static FembotGUI& getInstance() {
        static FembotGUI instance;
        return instance;
    }

    FembotGUI(const FembotGUI&) = delete;

    float fps = 60.f;
    float speed = 1.f;

    void init();
    void toggleGUI() {
        show_gui = !show_gui;
    }

    bool shouldRender() {
        return show_gui;
    }
};

void renderGUI();