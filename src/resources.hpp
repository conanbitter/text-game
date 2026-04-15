#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <filesystem>

class Resource {
public:
    virtual void load(const std::string& filename) {}
    virtual void destroy() {}
    bool isValid() { return valid; }
    ~Resource() { destroy(); };

protected:
    bool valid;
};

class ResourceManager {
public:
    template <typename T>
        requires std::derived_from<T, Resource>
    std::shared_ptr<T> get(const std::string& filename);

    void clear();

    std::filesystem::path root = "assets";

private:
    std::unordered_map<std::string, std::weak_ptr<Resource>> resources;
};
