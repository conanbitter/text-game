#include <filesystem>
#include "font.hpp"
#include "datafile.hpp"
#include "SDL3/SDL.h"

Font::~Font() {
    destroyFont();
}

void Font::load(const std::string& filename) {
    DataFile data(filename, "FONT"_c, 1);

    data.seek("HEAD"_c);
    size = data.readF32();
    lineHeight = data.readF32();
    range = data.readF32();
    spaceAdvance = data.readF32();

    data.seek("CHAR"_c);
    int glyphCount = data.sectionSize("CHAR"_c) / (8 * 4);
    for (uint32_t i = 0;i < glyphCount;i++) {
        GlyphData glyph;
        uint32_t code = data.readU32();
        glyph.rect.x = data.readF32();
        glyph.rect.y = data.readF32();
        glyph.rect.w = data.readF32();
        glyph.rect.h = data.readF32();
        glyph.origin.x = data.readF32();
        glyph.origin.y = data.readF32();
        glyph.advance = data.readF32();
        glyphs[code] = glyph;
    }

    data.seek("KERN"_c);
    int kpairsCount = data.sectionSize("KERN"_c) / (3 * 4);
    for (uint32_t i = 0;i < kpairsCount;i++) {
        uint32_t left = data.readU32();
        uint32_t right = data.readU32();
        float dist = data.readF32();
        kerning[{left, right}] = dist;
    }

    std::string textureFilename = data.readStr("IMAG"_c);
    std::filesystem::path folder = std::filesystem::path(filename).parent_path();
    texture.load((folder / textureFilename).string());

    valid = true;
#ifdef DEBUG        
    SDL_Log("Font \"%s\" loaded", filename.c_str());
#endif
}

void Font::destroy() {
    destroyFont();
    Resource::destroy();
}

GLuint Font::gerTextureId() {
    return texture.getId();
}

void Font::destroyFont() {
    if (!valid) return;
    texture.destroy();
    kerning.clear();
    glyphs.clear();
#ifdef DEBUG        
    SDL_Log("Font destroyed");
#endif
}
