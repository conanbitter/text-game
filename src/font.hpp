#pragma once

#include <unordered_map>
#include <map>
#include <memory>
#include "resources.hpp"
#include "textures.hpp"
#include "data.hpp"

struct GlyphData
{
    Rect rect;
    Point origin;
    float advance;
};

struct Font : public Resource, public ITextureContainer {
public:
    ~Font() override;
    void load(const std::string& filename) override;
    void destroy() override;
    GLuint gerTextureId() override;
private:
    float range;
    float spaceAdvance;
    float size;
    float lineHeight;

    std::unordered_map<uint32_t, GlyphData> glyphs;
    std::map<std::pair<uint32_t, uint32_t>, float> kerning;

    Texture texture;

    void destroyFont();
};

typedef std::shared_ptr<Font> PFont;
