#pragma once

#include "scene/Scene.hpp"
#include "serialization/SceneSerializer.hpp"

namespace rle
{
    class SceneManager
    {
    private:
        std::unique_ptr<Scene> active_scene_{nullptr};
        SceneSerializer serializer_;

        std::string startup_scene_path_;

    public:
        explicit SceneManager(NodeRegistry* node_registry);

        bool LoadScene(const std::string& path);
        void SaveScene(const std::string& path);
        void CreateDefaultScene() {active_scene_ = std::make_unique<Scene>();}

        void SetScene(std::unique_ptr<Scene> scene) {active_scene_ = std::move(scene);}
        Scene* GetScene() {return active_scene_.get();}
        
    };
}