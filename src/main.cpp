#include <SDL3/SDL_main.h>
#include "app.hpp"
#include "textures.hpp"

class MyApp :public App {
public:
    void load() override {
        tex = res.get<Texture>("assets/bitmap.png");
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