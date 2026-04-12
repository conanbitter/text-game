#pragma once

#include <SDL3/SDL.h>
#include <string>

class App;

class Window {
public:
    Window(App& parent);
    ~Window();
    void init(int width, int height, const char* title);
    void free();
    void processEvents();
    void present();
    void setTitle(const std::string& title);
    void setVirtualResolution(int width, int height);
    void disableVirtualResolution();

private:
    void resize(int newWidth, int newHeight);

    App& app;

    SDL_Window* window;
    SDL_Event e;
    //SDL_GLContext context;

    int virtualWidth = 0;
    int virtualHeight = 0;
    int width;
    int height;
};