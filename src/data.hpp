#pragma once

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

#pragma pack(pop)

struct Point {
    float x;
    float y;
};