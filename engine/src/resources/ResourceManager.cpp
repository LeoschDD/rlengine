#include "resources/ResourceManager.hpp"
#include "core/Log.hpp"

Shader *rle::ResourceManager::LoadShader(const std::string& name, const std::string& vs_path, const std::string& fs_path)
{
    if (shaders_.contains(name))
    {
        RLE_CORE_WARN("tried to load shader with a name that already exists: {}", name);
        return nullptr;
    }
    std::unique_ptr<Shader, ShaderDeleter> shader(new Shader(::LoadShader(vs_path.c_str(), fs_path.c_str())));
    shaders_.emplace(name, std::move(shader));
    return shaders_[name].get();
}

Texture *rle::ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{    
    if (textures_.contains(name))
    {
        RLE_CORE_WARN("tried to load texture with a name that already exists: {}", name);
        return nullptr;
    }
    std::unique_ptr<Texture, TextureDeleter> texture(new Texture(::LoadTexture(path.c_str())));
    textures_.emplace(name, std::move(texture));
    return textures_[name].get();
}
