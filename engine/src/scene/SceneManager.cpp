#include "scene/SceneManager.hpp"
#include "core/Log.hpp"
#include "nodes/3D/NodeCamera3D.hpp"
#include "nodes/2D/NodeCamera2D.hpp"

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

        if (!active_camera_) return;
        if (rle::NodeCamera2D* camera_2d = dynamic_cast<rle::NodeCamera2D*>(active_camera_))
        {
            BeginMode2D(camera_2d->GetCamera());
            active_scene_->Root()->Render2D();
            EndMode2D();
        }
        else if (rle::NodeCamera3D* camera_3d = dynamic_cast<rle::NodeCamera3D*>(active_camera_))
        {
            BeginMode3D(camera_3d->GetCamera());
            DrawGrid(5000, 5);
            active_scene_->Root()->Render3D();
            EndMode3D();
        }
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

void rle::SceneManager::SetActiveCamera(Node* camera)
{
    if (!dynamic_cast<rle::NodeCamera2D*>(camera) && 
        !dynamic_cast<rle::NodeCamera3D*>(camera) && camera)
    {
        RLE_CORE_WARN("failed to set camera - node is no camera");
        return;
    }
    if (active_camera_)
    {
        if (rle::NodeCamera2D* camera_2d = dynamic_cast<rle::NodeCamera2D*>(active_camera_))
        {
            camera_2d->SetCurrent(false);
        }
        else if (rle::NodeCamera3D* camera_3d = dynamic_cast<rle::NodeCamera3D*>(active_camera_))
        {
            camera_3d->SetCurrent(false);
        }
    }
    active_camera_ = camera;
    if (active_camera_)
    {
        if (rle::NodeCamera2D* camera_2d = dynamic_cast<rle::NodeCamera2D*>(active_camera_))
        {
            camera_2d->SetCurrent(true);
        }
        else if (rle::NodeCamera3D* camera_3d = dynamic_cast<rle::NodeCamera3D*>(active_camera_))
        {
            camera_3d->SetCurrent(true);
        }
    }
}