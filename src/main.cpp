#include <SDL3/SDL_main.h>
#include "app.hpp"
#include "textures.hpp"
#include <SDL3/SDL.h>

class MyApp :public App {
public:
    void load() override {
        tex = res.get<Texture>("assets/bitmap.png");
        int w, h;
        tex->getSize(w, h);
        SDL_Log("Tex1 size: %d x %d", w, h);
        PTexture tex2 = res.get<Texture>("assets/bitmap.png");
        tex2->getSize(w, h);
        SDL_Log("Tex2 size: %d x %d", w, h);
    }

    void draw() override {
        renderer.setBasicShader();
        renderer.setTexture(tex);
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