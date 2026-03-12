#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/SceneManager.hpp"
#include "serialization/NodeRegistry.hpp"
#include "core/Project.hpp"

namespace rle
{
    class Application
    {
    private:
        bool running_{false};
        
        NodeRegistry node_registry_;
        SceneManager scene_manager_;
    
    private:
        bool Init();
        void RegisterNodeTypes();
    
    public:
        Application();
        virtual ~Application();

        void Run();

        SceneManager& GetSceneManager() {return scene_manager_;}
    };
}