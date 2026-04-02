#include "taf.hpp"
#include <SDL3/SDL.h>
#include "window.hpp"

void taf::Window::setTitle(const std::string& title) {
    SDL_SetWindowTitle(window, title.c_str());
}

void taf::Window::setVirtualResolution(int width, int height) {
    virtualWidth = width;
    virtualHeight = height;
    resize();
}

void taf::Window::disableVirtualResolution() {
    virtualWidth = 0;
    resize();
}

void taf::Window::resize() {
}

void taf::ShowError(const std::exception& e) {
#ifdef DEBUG        
    SDL_Log(e.what());
#else
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
#endif
}
