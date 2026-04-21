#include <SDL3/SDL_main.h>
#include "app.hpp"
#include "textures.hpp"
#include "font.hpp"
#include <SDL3/SDL.h>
#include "datafile.hpp"

class MyApp :public App {
public:
    void load() override {
        tex = res.get<Texture>("bitmap.png");
        tex2 = res.get<Texture>("bitmap_transparent.png");
        font = res.get<Font>("font.fnt");
        window.setVirtualResolution(800, 600);
    }

    void draw() override {
        renderer.draw(tex, 10, 10);
        renderer.draw(tex2, 300, 10);
    }
private:
    PTexture tex;
    PTexture tex2;
    PFont font;
};

int main(int argc, char* argv[]) {
    try {
        MyApp app;
        app.run(800, 600, "TAF");
    }
    catch (const std::exception& e) {
        ShowError(e);
    }
    return 0;
}