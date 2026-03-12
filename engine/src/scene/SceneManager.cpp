#include "scene/SceneManager.hpp"
#include "core/Log.hpp"

void rle::SceneManager::ProcessInput()
{
    if (active_scene_ && active_scene_->Root())
    {
        active_scene_->Root()->Input();
    }
}

void rle::SceneManager::ProcessUpdate(const float dt)
{
    if (active_scene_ && active_scene_->Root())
    {
        active_scene_->Root()->Update(dt);
    }
}

void rle::SceneManager::ProcessRender()
{
    if (active_scene_ && active_scene_->Root())
    {
        active_scene_->Root()->Render();
    }
}

void rle::SceneManager::OnActivateScene()
{
    if (active_scene_ && active_scene_->Root())
    {
        active_scene_->Root()->EnterTree();
    }
}

void rle::SceneManager::OnDeactivateScene()
{
    if (active_scene_ && active_scene_->Root())
    {
        active_scene_->Root()->ExitTree();
    }
}

rle::SceneManager::SceneManager(NodeRegistry *node_registry)
    : serializer_(node_registry) 
{}

rle::SceneManager::~SceneManager()
{
    OnDeactivateScene();
}

bool rle::SceneManager::LoadScene(const std::string &path)
{
    auto scene = serializer_.DeserializeFromFile(path);
    if (!scene)
    {
        RLE_CORE_TRACE("failed to load scene");
        return false;
    }
    SetScene(std::move(scene));
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

void rle::SceneManager::SetScene(std::unique_ptr<Scene> scene)
{
    if (active_scene_)
    {
        OnDeactivateScene();
    }
    active_scene_ = std::move(scene);
    OnActivateScene();
}