#include "renderer.hpp"

Renderer::~Renderer() {
    destroy();
}

void Renderer::init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &spriteBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, spriteBuffer);
    glGenTextures(1, &spriteTexture);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_BUFFER, spriteTexture);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, spriteBuffer);
    glBindTexture(GL_TEXTURE_BUFFER, 0);

    glClearColor(0.6f, 0.7f, 0.8f, 1.0f);

    basicShader.init();
    sdfShader.init();
}

void Renderer::destroy() {
    glDeleteTextures(1, &spriteTexture);
    glDeleteBuffers(1, &spriteBuffer);
    glDeleteVertexArrays(1, &vao);
}

void Renderer::draw() {
    glBindVertexArray(vao);
    glBindBuffer(GL_TEXTURE_BUFFER, spriteBuffer);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(SpriteData) * sprites.size(), sprites.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_BUFFER, spriteTexture);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6 * sprites.size());
}
