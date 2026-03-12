#pragma once

#include "core/Headers.hpp"

namespace rle
{
    class ResourceManager
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_;
        std::unordered_map<std::string, std::unique_ptr<Texture>> textures_;

    public:
        ResourceManager() = default;

        Shader* LoadShader(const std::string& name, const std::string& vs_path, const std::string& fs_path);
        Texture* LoadTexture(const std::string& name, const std::string& path);
    };
}