#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>

namespace taf {

    class Window {
    public:
        void setTitle(const std::string& title);
        void setVirtualResolution(int width, int height);
        void disableVirtualResolution();

    private:
        void resize();

        const App& app;

        SDL_Window* window;
        SDL_GLContext context;

        int virtualWidth = 0;
        int virtualHeight = 0;
        int width;
        int height;
    };

    void ShowError(const std::exception& e);
}

#endif