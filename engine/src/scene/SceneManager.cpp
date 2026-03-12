#include "scene/SceneManager.hpp"
#include "core/Log.hpp"

void rle::SceneManager::LoadScene(const std::string& path)
{
    auto scene = serializer_.DeserializeFromFile(path);
    if (!scene)
    {
        RLE_CORE_ERROR("failed to load scene");
        return;
    }
    active_scene_ = std::move(scene);
}

void rle::SceneManager::SaveScene(const std::string& path)
{
    if (!active_scene_)
    {
        RLE_CORE_ERROR("failed to save scene - no active scene");
        return;
    }
    serializer_.SerializeToFile(active_scene_.get(), path);
}