#include "shaders.hpp"
#include <fstream>
#include <SDL3/SDL.h>

void loadFromFile(const char* filename) {
    std::ifstream infile(filename);
    if (!infile) {
        SDL_Log("Cannot load shader file \"%s\"\n", filename);
        return;
    }
    std::string shader_text((std::istreambuf_iterator<char>(infile)),
        std::istreambuf_iterator<char>());
    SDL_Log("Shader text \n%s\n", shader_text.c_str());
}