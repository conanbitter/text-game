#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "textures.hpp"
#include <stdexcept>
#include <format>

void Texture::destroy() {
    if (glIsTexture(handle)) {
        glDeleteTextures(1, &handle);
    }
    valid = false;
}

void Texture::load(const std::string& filename) {
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
    void* data = stbi_load(filename.c_str(), &w, &h, &c, 4);
    if (data == nullptr) {
        throw std::runtime_error(std::format("Error loading texture \"{}\" : {}", filename, stbi_failure_reason()));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    width = w;
    height = h;
    valid = true;
}

