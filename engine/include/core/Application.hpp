#pragma once

#include "core/Headers.hpp"
#include "core/Log.hpp"
#include "scene/SceneTree.hpp"
#include "nodes/NodeRegistry.hpp"

namespace rle
{
    class Application
    {
    private:
        bool running_{false};
        
        SceneTree scene_tree_;
        NodeRegistry node_registry_;

    private:
        bool Init();
        void RegisterNodeTypes();

    protected:
        virtual bool OnInit() {return true;}

    public:
        Application();
        ~Application() = default;

        void Run();
    };
}