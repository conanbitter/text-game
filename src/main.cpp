#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <utility>

#include "utf8.h"

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
    float roundness;
    float blur;
};

struct SpriteData {
    Rect     dst;      // cell 1
    Rect     src;      // cell 2
    Color    color;    // cell 3
    FontData fontData; // cell 4
};
#pragma pack(pop)

struct Point {
    float x;
    float y;
};

struct Size {
    float w;
    float h;
};

struct GlyphData
{
    Rect rect;
    Point origin;
    float advance;
};

struct Font {
    float range;
    float spaceAdvance;
    std::unordered_map<uint32_t, GlyphData> glyphs;
    std::map<std::pair<uint32_t, uint32_t>, float> kerning;
};

const uint32_t DEFAULT_GLYPH = 35; // '#'

SDL_Window* window;
SDL_GLContext context;
SDFShader shader;
int virtualWidth = 800;
int virtualHeight = 600;

std::vector<SpriteData> sprites;

float readFloat(std::ifstream& file) {
    float result;
    file.read(reinterpret_cast<char*>(&result), sizeof(float));
    return result;
}

uint32_t readUint32(std::ifstream& file) {
    uint32_t result;
    file.read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
    return result;
}

Font loadFont(const char* filename) {
    Font font;

    std::ifstream file(filename, std::ios::in | std::ios::binary);
    font.range = readFloat(file);
    font.spaceAdvance = readFloat(file);

    uint32_t glyphCount = readUint32(file);
    uint32_t kpairsCount = readUint32(file);
    for (uint32_t i = 0;i < glyphCount;i++) {
        GlyphData data;
        uint32_t code = readUint32(file);
        data.rect.x = readFloat(file);
        data.rect.y = readFloat(file);
        data.rect.w = readFloat(file);
        data.rect.h = readFloat(file);
        data.origin.x = readFloat(file);
        data.origin.y = readFloat(file);
        data.advance = readFloat(file);
        font.glyphs[code] = data;
    }

    for (uint32_t i = 0;i < kpairsCount;i++) {
        uint32_t left = readUint32(file);
        uint32_t right = readUint32(file);
        float dist = readFloat(file);
        font.kerning[{left, right}] = dist;
    }

    return font;
}

void printText(const char* text, float x, float y, float scale, Font& font) {
    std::string teststring(text);
    std::vector<SpriteData> letters;
    std::vector<SpriteData> shadows;
    uint32_t prev = 0;

    utf8::iterator it(teststring.begin(), teststring.begin(), teststring.end());
    utf8::iterator endit(teststring.end(), teststring.begin(), teststring.end());
    for (auto i = it;i != endit;++i) {
        uint32_t code = *i;

        if (code == 32) { // space
            x += font.spaceAdvance * scale;
            continue;
        }

        auto glyph = font.glyphs.find(code);
        if (glyph == font.glyphs.end()) glyph = font.glyphs.find(DEFAULT_GLYPH);
        GlyphData& gd = glyph->second;

        auto kp = font.kerning.find({ prev, code });
        if (kp != font.kerning.end()) {
            x += kp->second * scale;
        }

        Rect dst = Rect{
            .x = x - gd.origin.x * scale,
            .y = y - gd.origin.y * scale,
            .w = gd.rect.w * scale,
            .h = gd.rect.h * scale
        };

        shadows.push_back(SpriteData{
        .dst = dst,
        .src = gd.rect,
        .color = Color{.r = 0.2, .g = 0.4, .b = 0.6, .a = 1.0},
        .fontData = FontData{.scale = scale , .thickness = 2.0f, .roundness = 1.0f, .blur = 0.0f},
            });

        letters.push_back(SpriteData{
        .dst = dst,
        .src = gd.rect,
        .color = Color{.r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0},
        .fontData = FontData{.scale = scale , .thickness = 0.0f, .roundness = 0.0f, .blur = 0.0f},
            });

        x += gd.advance * scale;
        prev = code;
    }
    sprites.insert(sprites.end(), shadows.begin(), shadows.end());
    sprites.insert(sprites.end(), letters.begin(), letters.end());
}

void resize(int newWidth, int newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    //shader.setViewport(0, 0, e.window.data1, e.window.data2);
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

    shader.setViewport(xOffset, yOffset, virtualWidth, virtualHeight);
    shader.setScale(scale);
}

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

    shader.init();
    Texture texture;
    texture.load("assets/font.png");
    shader.setRange(8);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    Font font = loadFont("assets/font.fnt");

    float border = 2.0; // MAX_BORDER = range*scale/2 - 1;
    int tw, th;
    texture.getSize(tw, th);
    float lh = 35.0;
    float y = 12.0 + lh * 2.0;
    const char* message = "AW To Hello world! Привет мир!";
    printText(message, 100, y, 2.0f, font);y += lh * 2.0f;
    printText(message, 100, y, 1.8f, font);y += lh * 1.8f;
    printText(message, 100, y, 1.6f, font);y += lh * 1.6f;
    printText(message, 100, y, 1.4f, font);y += lh * 1.4f;
    printText(message, 100, y, 1.2f, font);y += lh * 1.2f;
    printText(message, 100, y, 1.0f, font);y += lh * 1.0f;
    printText(message, 100, y, 0.9f, font);y += lh * 0.9f;
    printText(message, 100, y, 0.8f, font);y += lh * 0.8f;
    printText(message, 100, y, 0.7f, font);y += lh * 0.7f;
    printText(message, 100, y, 0.6f, font);y += lh * 0.6f;
    printText(message, 100, y, 0.5f, font);y += lh * 0.5f;
    printText(message, 100, y, 0.4f, font);y += lh * 0.4f;
    printText(message, 100, y, 0.3f, font);y += lh * 0.3f;
    printText(message, 100, y, 0.2f, font);y += lh * 0.2f;
    printText(message, 100, y, 0.1f, font);y += lh * 0.1f;
    /*sprites.push_back(SpriteData{
        .dst = Rect{.x = 16,.y = 16,.w = 256,.h = 256},
        .src = Rect{.x = 0,.y = 0,.w = (float)tw, .h = (float)th},
        .color = Color{.r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0},
        .fontData = FontData{.scale = 256.0f / tw , .thickness = border, .roundness = 1.0, .blur = 0.0},
        });
    sprites.push_back(SpriteData{
        .dst = Rect{.x = 273,.y = 16,.w = 512,.h = 512},
        .src = Rect{.x = 0,.y = 0,.w = (float)tw, .h = (float)th},
        .color = Color{.r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0},
        .fontData = FontData{.scale = 512.0f / tw, .thickness = border, .roundness = 1.0, .blur = 0.0},
        });
    sprites.push_back(SpriteData{
        .dst = Rect{.x = 144,.y = 273,.w = 128,.h = 128},
        .src = Rect{.x = 0,.y = 0,.w = (float)tw, .h = (float)th},
        .color = Color{.r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0},
        .fontData = FontData{.scale = 128.0f / tw , .thickness = border, .roundness = 1.0, .blur = 0.0},
        });

    sprites.push_back(SpriteData{
        .dst = Rect{.x = 16,.y = 16,.w = 256,.h = 256},
        .src = Rect{.x = 0,.y = 0,.w = (float)tw, .h = (float)th},
        .color = Color{.r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0},
        .fontData = FontData{.scale = 256.0f / tw, .thickness = 0.0, .roundness = 0.0},
        });
    sprites.push_back(SpriteData{
        .dst = Rect{.x = 273,.y = 16,.w = 512,.h = 512},
        .src = Rect{.x = 0,.y = 0,.w = (float)tw, .h = (float)th},
        .color = Color{.r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0},
        .fontData = FontData{.scale = 512.0f / tw, .thickness = 0.0, .roundness = 0.0},
        });
    sprites.push_back(SpriteData{
        .dst = Rect{.x = 144,.y = 273,.w = 128,.h = 128},
        .src = Rect{.x = 0,.y = 0,.w = (float)tw, .h = (float)th},
        .color = Color{.r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0},
        .fontData = FontData{.scale = 128.0f / tw, .thickness = 0.0, .roundness = 0.0},
        });*/
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

    resize(800, 600);

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

        glBindVertexArray(vao);
        texture.bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_BUFFER, spriteTexture);
        shader.use();
        glClearColor(0.6f, 0.7f, 0.8f, 1.0f);
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
#ifdef DEBUG        
        SDL_Log(e.what());
#else
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
#endif        
    }
    return 0;
}