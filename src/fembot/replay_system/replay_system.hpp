class FembotReplaySystem {
protected:
    FembotReplaySystem() {}

    float fps = 60.f;
public:
    static FembotReplaySystem& getInstance() {
        static FembotReplaySystem instance;
        return instance;
    }

    FembotReplaySystem(const FembotReplaySystem&) = delete;

    float getFPS() const {
        return fps;
    }

    void setFPS(float fps) {
        this->fps = fps;
    }
};