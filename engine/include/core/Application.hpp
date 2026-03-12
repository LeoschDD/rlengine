#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/SceneManager.hpp"
#include "serialization/NodeRegistry.hpp"

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

    protected:
        virtual bool OnInit() {return true;}
        virtual void RegisterCustomNodeTypes(NodeRegistry& node_registry) {}

    public:
        Application();
        virtual ~Application();

        void Run();

        SceneManager& GetSceneManager() {return scene_manager_;}
    };
}