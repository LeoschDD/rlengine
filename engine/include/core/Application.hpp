#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/SceneManager.hpp"
#include "serialization/NodeRegistry.hpp"
#include "core/Project.hpp"
#include "resources/ResourceManager.hpp"

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
        virtual void ProcessEditor() {}

    public:
        Application();
        virtual ~Application();

        static Application& Get() {return *instance_;}

        void Run();

        SceneManager& GetSceneManager() {return scene_manager_;}
        ResourceManager& GetResourceManager() {return resource_manager_;}
    };
}