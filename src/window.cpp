#include <format>
#include "window.hpp"
#include "app.hpp"

void Window::init(int width, int height, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::format("SDL could not initialize!SDL_Error: \"{}\"", SDL_GetError()));
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (window == NULL) {
        throw std::runtime_error(std::format("Window could not be created! SDL_Error: \"{}\"", SDL_GetError()));
    }

    resize(width, height);
}

void Window::free() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::startLoop() {
    quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                resize(e.window.data1, e.window.data2);
                break;

            case SDL_EVENT_KEY_DOWN:
                switch (e.key.key)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;

                default:
                    break;
                }

            default:
                break;
            }
        }

        app.update();

        app.draw();

        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }
}

void Window::setTitle(const std::string& title) {
    SDL_SetWindowTitle(window, title.c_str());
}
/*
void Window::setVirtualResolution(int width, int height) {
    virtualWidth = width;
    virtualHeight = height;
    resize();
}

void Window::disableVirtualResolution() {
    virtualWidth = 0;
    resize();
}*/

void Window::resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

void ShowError(const std::exception& e) {
#ifdef DEBUG        
    SDL_Log(e.what());
#else
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
#endif
}
