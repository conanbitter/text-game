#pragma once

#include "window.hpp"
#include "renderer.hpp"
#include "resources.hpp"

class App {
public:
    App();
    virtual void load() {}
    virtual void update() {}
    virtual void draw() {}

    void run(int initialWidth, int initialHeight, const char* title);
    void requestQuit();

protected:
    Window window;
    Renderer renderer;
    ResourceManager res;
    bool working;
};

void ShowError(const std::exception& e);