#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/SceneManager.hpp"
#include "serialization/NodeRegistry.hpp"
#include "core/Project.hpp"
#include "resources/ResourceManager.hpp"
#include "nodes/2D/Node2D.hpp"
#include "nodes/3D/Node3D.hpp"
#include "nodes/3D/NodeMesh3D.hpp"

namespace rle
{
    class Application
    {
    private:
        static Application* instance_;
        
        bool running_{false};
        
        NodeRegistry node_registry_;
        SceneManager scene_manager_;
        ResourceManager resource_manager_;

    private:
        bool Init();
        void RegisterNodeTypes();
    
    protected:
        virtual void Input();
        virtual void Update(const float dt);
        virtual void Render();

    public:
        Application();
        virtual ~Application();

        static Application& Get() {return *instance_;}

        void Run();

        NodeRegistry& GetNodeRegistry() {return node_registry_;}
        SceneManager& GetSceneManager() {return scene_manager_;}
        ResourceManager& GetResourceManager() {return resource_manager_;}
    };
}