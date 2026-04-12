#include "app.hpp"

App::App() :window(*this) {}

void App::run(int initialWidth, int initialHeight, const char* title) {
    window.init(initialWidth, initialHeight, title);
    renderer.init();

    load();

    working = true;

    while (working) {
        window.processEvents();

        update();

        draw();

        renderer.draw();
        window.present();
    }

    renderer.destroy();
    window.destroy();
}

void App::requestQuit() {
    working = false;
}

void ShowError(const std::exception& e) {
#ifdef DEBUG        
    SDL_Log(e.what());
#else
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
#endif
}
