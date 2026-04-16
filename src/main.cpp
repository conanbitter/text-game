#include <SDL3/SDL_main.h>
#include "app.hpp"
#include "textures.hpp"
#include <SDL3/SDL.h>

class MyApp :public App {
public:
    void load() override {
        tex = res.get<Texture>("assets/bitmap.png");
        window.setVirtualResolution(800, 600);
    }

    void draw() override {
        renderer.draw(tex, 10, 10);
        renderer.draw(tex, 300, 10);
    }
private:
    PTexture tex;
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