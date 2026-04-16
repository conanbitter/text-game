#include <format>
#include "glad/gl.h"
#include "window.hpp"
#include "app.hpp"

Window::Window(App& parent) :app{ parent } {}

Window::~Window() {
    destroy();
}

void Window::init(int width, int height, const char* title) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::format("SDL could not initialize! SDL_Error: \"{}\"", SDL_GetError()));
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (window == NULL) {
        throw std::runtime_error(std::format("Window could not be created! SDL_Error: \"{}\"", SDL_GetError()));
    }

    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        throw std::runtime_error(std::format("OpenGL context could not be created! SDL_Error: \"{}\"", SDL_GetError()));
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
#ifdef DEBUG        
    SDL_Log("Using OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#endif

    this->width = width;
    this->height = height;
}

void Window::destroy() {
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::processEvents() {
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type)
        {
        case SDL_EVENT_QUIT:
            app.requestQuit();
            break;

        case SDL_EVENT_WINDOW_RESIZED:
            resize(e.window.data1, e.window.data2);
            break;

        case SDL_EVENT_KEY_DOWN:
            switch (e.key.key)
            {
            case SDLK_ESCAPE:
                app.requestQuit();
                break;

            default:
                break;
            }

        default:
            break;
        }
    }
}

void Window::present() {
    SDL_GL_SwapWindow(window);
    SDL_Delay(1);
}

void Window::setTitle(const std::string& title) {
    SDL_SetWindowTitle(window, title.c_str());
}

void Window::setVirtualResolution(int width, int height) {
    virtualWidth = width;
    virtualHeight = height;
    resize(this->width, this->height);
}

void Window::disableVirtualResolution() {
    virtualWidth = 0;
    resize(width, height);
}

void Window::updateSize() {
    resize(width, height);
}

void Window::resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;

    glViewport(0, 0, newWidth, newHeight);

    if (virtualWidth == 0) {
        app.updateViewport(0.0f, 0.0f, newWidth, newHeight, 1.0f);
    } else {
        float xOffset = 0.0f;
        float yOffset = 0.0f;
        float scale = 1.0f;

        float windowAR = (float)newWidth / newHeight;
        float virtualAR = (float)virtualWidth / virtualHeight;

        if (virtualAR < windowAR) {
            float realWidth = virtualHeight * windowAR;
            xOffset = (realWidth - virtualWidth) / 2.0f;
            scale = (float)newHeight / virtualHeight;
        }
        if (virtualAR > windowAR) {
            float realHeight = virtualWidth / windowAR;
            yOffset = (realHeight - virtualHeight) / 2.0f;
            scale = (float)newWidth / virtualWidth;
        }

        app.updateViewport(xOffset, yOffset, virtualWidth, virtualHeight, scale);
    }
}