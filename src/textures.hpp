#pragma once

#include <glad/gl.h>
#include <string>
#include "resources.hpp"
#include <memory>

class Texture :public Resource {
private:
    int width;
    int height;
    GLuint handle;
    void destroyTexture();

public:
    Texture() : width{ 0 }, height{ 0 } {};
    ~Texture() override;
    void load(const std::string& filename) override;
    void destroy() override;
    void getSize(int& w, int& h) {
        w = width;
        h = height;
    }
    GLuint getId() const { return handle; }

    Texture(const Texture& prog) = delete;
    Texture& operator=(const Texture& prog) = delete;
    Texture(Texture&& prog) = delete;
    Texture& operator=(Texture&& prog) = delete;
};

class ITextureContainer {
public:
    virtual GLuint gerTextureId() = 0;
    virtual ~ITextureContainer() {}
};

typedef std::shared_ptr<Texture> PTexture;