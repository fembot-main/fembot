class FembotGUI {
protected:
    FembotGUI() {}

public:
    static FembotGUI& getInstance() {
        static FembotGUI instance;
        return instance;
    }

    FembotGUI(const FembotGUI&) = delete;

    float fps = 60.f;

    void render();
    void init();
};