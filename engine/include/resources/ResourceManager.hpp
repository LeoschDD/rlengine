#pragma once

#include "core/Headers.hpp"

namespace rle
{
    struct ShaderDeleter
    {
        void operator()(Shader* s) const
        {
            if (s) {UnloadShader(*s); delete s;}
        }
    };

    struct TextureDeleter
    {
        void operator()(Texture* t) const
        {
            if (t) {UnloadTexture(*t); delete t;}
        }
    };

    class ResourceManager
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<Shader, ShaderDeleter>> shaders_;
        std::unordered_map<std::string, std::unique_ptr<Texture, TextureDeleter>> textures_;

    public:
        ResourceManager() = default;

        Shader* LoadShader(const std::string& name, const std::string& vs_path, const std::string& fs_path);
        Texture* LoadTexture(const std::string& name, const std::string& path);
    };
}