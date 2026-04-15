#include "resources.hpp"
#include "textures.hpp"

template <typename T>
    requires std::derived_from<T, Resource>
std::shared_ptr<T> ResourceManager::get(const std::string& filename) {

    /*auto it = resources.find(key);
    if (it != resources.end()) {
        return static_cast<std::shared_ptr<T>>(it->second.get());
    }*/

    std::shared_ptr<T> result = std::make_shared<T>();
    result->load(filename);
    return result;
}

void ResourceManager::clear() {
}

template std::shared_ptr<Texture> ResourceManager::get<Texture>(const std::string& filename);
