#include "SandboxModule.hpp"
#include "PlayerNode.hpp"

namespace sandbox
{
    void RegisterNodeTypes(rle::NodeRegistry& registry)
    {
        registry.RegisterType("PlayerNode", []()
        {
            return std::make_unique<PlayerNode>();
        });
    }

    std::unique_ptr<rle::Scene> CreateStartupScene()
    {
        auto scene = std::make_unique<rle::Scene>();
        auto player = std::make_unique<PlayerNode>();

        scene->Root()->AddChild(std::move(player));
        return scene;
    }
}