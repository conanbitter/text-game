#ifndef TAF_HPP
#define TAF_HPP

#include "app.hpp"
#include "window.hpp"

namespace taf {

#pragma pack(push, 1)
    struct Rect {
        float x;
        float y;
        float w;
        float h;
    };

    struct Color {
        float r;
        float g;
        float b;
        float a;
    };

    struct FontData {
        float scale;
        float thickness;
        float roundness;
        float blur;
    };

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

}

#endif