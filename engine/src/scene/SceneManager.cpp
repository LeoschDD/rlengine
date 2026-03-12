#include "scene/SceneManager.hpp"
#include "core/Log.hpp"

rle::SceneManager::SceneManager(NodeRegistry *node_registry)
    : serializer_(node_registry) 
{}

bool rle::SceneManager::LoadScene(const std::string& path)
{
    auto scene = serializer_.DeserializeFromFile(path);
    if (!scene)
    {
        RLE_CORE_TRACE("failed to load scene");
        return false;
    }
    active_scene_ = std::move(scene);
    return true;
}

void rle::SceneManager::SaveScene(const std::string& path)
{
    if (!active_scene_)
    {
        RLE_CORE_ERROR("can't save scene - no active scene");
        return;
    }
    serializer_.SerializeToFile(active_scene_.get(), path);
}