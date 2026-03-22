#pragma once

#include <glad/gl.h>
#include <string>

class Texture {
private:
    int width;
    int height;
    GLuint handle;

public:
    Texture() : width{ 0 }, height{ 0 } {};
    ~Texture();
    void load(const char* filename);
    void bind();
    void getSize(int& w, int& h) {
        w = width;
        h = height;
    }

    Texture(const Texture& prog) = delete;
    Texture& operator=(const Texture& prog) = delete;
    Texture(Texture&& prog);
    Texture& operator=(Texture&& prog);
};