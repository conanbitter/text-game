#include "resources.hpp"
#include "textures.hpp"
#include "SDL3/SDL.h"

template <typename T>
    requires std::derived_from<T, Resource>
std::shared_ptr<T> ResourceManager::get(const std::string& filename) {

    auto it = resources.find(filename);
    if (it != resources.end()) {
        if (std::shared_ptr<Resource> entry = it->second.lock()) {
            std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(entry);
            if (result->isValid()) {
#ifdef DEBUG        
                SDL_Log("Getting resource \"%s\" from cache", filename.c_str());
#endif
                return result;
            }
        }
    }

#ifdef DEBUG        
    SDL_Log("Loading resource \"%s\"", filename.c_str());
#endif
    std::shared_ptr<T> result = std::make_shared<T>();
    result->load(filename);
    resources[filename] = result;
    return result;
}

void ResourceManager::clear() {
}

template std::shared_ptr<Texture> ResourceManager::get<Texture>(const std::string& filename);
