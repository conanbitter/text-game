#include "app.hpp"

App::App() :window(*this) {}

void App::run(int initialWidth, int initialHeight, const char* title) {
    window.init(initialWidth, initialHeight, title);
    renderer.init();
    window.updateSize();

    load();

    working = true;

    while (working) {
        window.processEvents();

        update();

        renderer.beginDrawing();
        draw();
        renderer.finishDrawing();

        window.present();
    }

    renderer.destroy();
    window.destroy();
}

void App::requestQuit() {
    working = false;
}

void App::updateViewport(float xOffset, float yOffset, float width, float height, float scale) {
    renderer.updateViewport(xOffset, yOffset, width, height, scale);
}

void ShowError(const std::exception& e) {
#ifdef DEBUG        
    SDL_Log(e.what());
#else
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
#endif
}
