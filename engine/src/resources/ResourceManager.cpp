#include "resources/ResourceManager.hpp"

Shader *rle::ResourceManager::LoadShader(const std::string& name, const std::string& vs_path, const std::string& fs_path)
{
    auto shader = std::make_unique<Shader>(::LoadShader(vs_path.c_str(), fs_path.c_str()));
    shaders_.emplace(name, std::move(shader));
    return shaders_[name].get();
}

Texture *rle::ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
    auto texture = std::make_unique<Texture>(::LoadTexture(path.c_str()));
    textures_.emplace(name, std::move(texture));
    return textures_[name].get();
}
