#pragma once

#include <rlengine.hpp>
#include "PlayerNode.hpp"

class SandboxProject : public rle::Project
{
public:
    void RegisterNodeTypes(rle::NodeRegistry& registry) override
    {
        registry.RegisterType("PlayerNode", [](){return std::make_unique<PlayerNode>();});
    }

    std::string GetStartupScenePath() const override
    {
        return std::format(SCENE_DIR "/main.rlscene");
    }

    std::unique_ptr<rle::Scene> CreateStartupScene() override
    {
        auto scene = std::make_unique<rle::Scene>("main");
        scene->Root()->AddChild(std::make_unique<PlayerNode>());
        return scene;
    }
};