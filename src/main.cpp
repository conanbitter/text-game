#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

#include "shaders.hpp"
#include "textures.hpp"

#pragma pack(push, 1)
struct Rect {
    float x;
    float y;
    float w;
    float h;
};

struct Color {
    float r;
    float g;
    float b;
    float a;
};

struct FontData {
    float scale;
    float thickness;
    float c;
    float d;
};

struct SpriteData {
    Rect     dst;      // cell 1
    Rect     src;      // cell 2
    Color    color;    // cell 3
    FontData fontData; // cell 4
};
#pragma pack(pop)

SDL_Window* window;
SDL_GLContext context;

std::vector<SpriteData> sprites;

void run() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Text Game", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        SDL_Log("Context could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    SDL_Log("Using OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    SDFShader shader;
    shader.init();
    Texture texture;
    texture.load("assets/sdf_test32.png");
    shader.setRange(32);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float border = 3.0; // MAX_BORDER = range*scale/2 - 1;

    sprites.push_back(SpriteData{
        .dst = Rect{.x = 16,.y = 16,.w = 256,.h = 256},
        .src = Rect{.x = 0,.y = 0,.w = 256, .h = 256},
        .color = Color{.r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0},
        .fontData = FontData{.scale = 1.0, .thickness = border},
        });

    sprites.push_back(SpriteData{
        .dst = Rect{.x = 273,.y = 16,.w = 512,.h = 512},
        .src = Rect{.x = 0,.y = 0,.w = 256, .h = 256},
        .color = Color{.r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0},
        .fontData = FontData{.scale = 2.0, .thickness = border},
        });
    sprites.push_back(SpriteData{
        .dst = Rect{.x = 144,.y = 273,.w = 128,.h = 128},
        .src = Rect{.x = 0,.y = 0,.w = 256, .h = 256},
        .color = Color{.r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0},
        .fontData = FontData{.scale = 0.5, .thickness = border},
        });

    sprites.push_back(SpriteData{
        .dst = Rect{.x = 16,.y = 16,.w = 256,.h = 256},
        .src = Rect{.x = 0,.y = 0,.w = 256, .h = 256},
        .color = Color{.r = 0.808, .g = 0.576, .b = 0.847, .a = 1.0},
        .fontData = FontData{.scale = 1.0, .thickness = 0.0},
        });
    sprites.push_back(SpriteData{
        .dst = Rect{.x = 273,.y = 16,.w = 512,.h = 512},
        .src = Rect{.x = 0,.y = 0,.w = 256, .h = 256},
        .color = Color{.r = 0.690, .g = 0.745, .b = 0.773, .a = 1.0},
        .fontData = FontData{.scale = 2.0, .thickness = 0.0},
        });
    sprites.push_back(SpriteData{
        .dst = Rect{.x = 144,.y = 273,.w = 128,.h = 128},
        .src = Rect{.x = 0,.y = 0,.w = 256, .h = 256},
        .color = Color{.r = 1.0, .g = 0.718, .b = 0.302, .a = 1.0},
        .fontData = FontData{.scale = 0.5, .thickness = 0.0},
        });
    GLuint spriteBuffer;
    glGenBuffers(1, &spriteBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, spriteBuffer);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(SpriteData) * sprites.size(), sprites.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
    GLuint spriteTexture;
    glGenTextures(1, &spriteTexture);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_BUFFER, spriteTexture);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, spriteBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    shader.setViewport(0, 0, 800, 600);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                glViewport(0, 0, e.window.data1, e.window.data2);
                shader.setViewport(0, 0, e.window.data1, e.window.data2);
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

        glBindVertexArray(vao);
        texture.bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_BUFFER, spriteTexture);
        shader.use();
        glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6 * sprites.size());
        SDL_GL_SwapWindow(window);

        SDL_Delay(1);
    }

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    try {
        run();
    }
    catch (const std::exception& e) {
        SDL_Log(e.what());
    }
    return 0;
}