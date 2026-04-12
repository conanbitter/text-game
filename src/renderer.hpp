#pragma once

#include <vector>
#include "data.hpp"
#include "glad/gl.h"

#pragma pack(push, 1)
union ExtraData
{
    FontData fontData;
};

struct SpriteData {
    Rect      dst;   // cell 1
    Rect      src;   // cell 2
    Color     color; // cell 3
    ExtraData extra; // cell 4
};
#pragma pack(pop)

class Renderer {
public:
    ~Renderer();
    void init();
    void destroy();
    void draw();
private:
    GLuint vao;
    GLuint spriteBuffer;
    GLuint spriteTexture;
    std::vector<SpriteData> sprites;
};