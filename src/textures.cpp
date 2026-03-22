#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "textures.hpp"
#include <stdexcept>
#include <format>

Texture::~Texture() {
    if (glIsTexture(handle)) {
        glDeleteTextures(1, &handle);
    }
}

void Texture::load(const char* filename) {
    if (glIsTexture(handle)) {
        glDeleteTextures(1, &handle);
    }

    glGenTextures(1, &handle);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, c;
    stbi_set_flip_vertically_on_load(1);
    void* data = stbi_load(filename, &w, &h, &c, 4);
    if (data == nullptr) {
        throw std::runtime_error(std::format("Error loading texture: \"{}\"", stbi_failure_reason()));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    width = w;
    height = h;
}

void Texture::bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle);
}

Texture::Texture(Texture&& tex) : handle{ tex.handle }, width{ tex.width }, height{ tex.height } {
    tex.width = 0;
    tex.height = 0;
}

Texture& Texture::operator=(Texture&& tex) {
    if (glIsTexture(handle)) {
        glDeleteTextures(1, &handle);
    }
    handle = tex.handle;
    width = tex.width;
    height = tex.height;
    tex.width = 0;
    tex.height = 0;
    return *this;
}
