#pragma once

#include "window.hpp"

class App {
public:
    App();
    virtual void load() {}
    virtual void update() {}
    virtual void draw() {}

    void run(int initialWidth, int initialHeight, const char* title);
    void requestQuit();

private:
    Window window;
    bool working;
};

void ShowError(const std::exception& e);