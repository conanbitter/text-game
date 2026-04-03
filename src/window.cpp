#include <SDL3/SDL.h>
#include "window.hpp"

void Window::setTitle(const std::string& title) {
    SDL_SetWindowTitle(window, title.c_str());
}

void Window::setVirtualResolution(int width, int height) {
    virtualWidth = width;
    virtualHeight = height;
    resize();
}

void Window::disableVirtualResolution() {
    virtualWidth = 0;
    resize();
}

void Window::resize() {
}

void ShowError(const std::exception& e) {
#ifdef DEBUG        
    SDL_Log(e.what());
#else
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
#endif
}
