#pragma once

#include "scene/Scene.hpp"
#include "serialization/SceneSerializer.hpp"

namespace rle
{
    class Application;

    class SceneManager
    {
    private:
        friend class Application;

        std::unique_ptr<Scene> active_scene_{nullptr};
        SceneSerializer serializer_;
        
        Node* active_camera_{nullptr};

    private:
        void ProcessInput();
        void ProcessUpdate(const float dt);
        void ProcessRender();

        void OnActivateScene();
        void OnDeactivateScene();

    public:
        explicit SceneManager(NodeRegistry* node_registry);
        ~SceneManager();

        bool LoadScene(const std::string& path);
        void SaveScene(const std::string& path);

        void CreateDefaultScene() {active_scene_ = std::make_unique<Scene>();}

        void SetScene(std::unique_ptr<Scene> scene);
        Scene* GetScene() {return active_scene_.get();}
        void SetActiveCamera(Node* camera);
    };
}