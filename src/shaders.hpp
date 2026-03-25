#pragma once
#include <glad/gl.h>

class ShaderProgram {
public:
    void use();
    virtual void init() {}

    ~ShaderProgram();
protected:
    GLuint program = 0;
    void loadFromString(const char* vertexShaderCode, const char* fragmentShaderCode);
    void loadFromFiles(const char* vertexShaderFile, const char* fragmentShaderFile);
};

class BasicShader :public ShaderProgram {
public:
    void init() override;
    void setViewport(int xOffset, int yOffset, int width, int height);
protected:
    virtual void initUniforms();
private:
    GLint viewportUniform;
};

class SDFShader :public BasicShader {
public:
    void init() override;
    void setRange(float range);
    void setScale(float scale);
private:
    GLint rangeUniform;
    GLint scaleUniform;
    void initUniforms() override;
};