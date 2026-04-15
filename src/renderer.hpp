#pragma once

#include <vector>
#include "data.hpp"
#include "glad/gl.h"
#include "shaders.hpp"
#include "textures.hpp"
#include <limits>

const GLuint MAX_GL_UINT = std::numeric_limits<GLuint>::max();

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
    void beginDrawing();
    void finishDrawing();
    void setTexture(const std::shared_ptr<Texture>& texture);

    void setBasicShader();
    void setSdfShader();
private:
    GLuint vao;
    GLuint spriteBuffer;
    GLuint spriteTexture;
    std::vector<SpriteData> sprites;

    BasicShader basicShader;
    SDFShader sdfShader;

    GLuint currentShader;
    GLuint currentTexture;

    void setShader(const ShaderProgram& shader);
};