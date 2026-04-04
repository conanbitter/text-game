#include <SDL3/SDL_main.h>
#include "app.hpp"

int main(int argc, char* argv[]) {
    try {
        App app;
        app.run(800, 600, "TAF");
    }
    catch (const std::exception& e) {
        ShowError(e);
    }
    return 0;
}