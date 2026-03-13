#pragma once

#include <rlengine.hpp>
#include "PlayerNode.hpp"

class SandboxProject : public rle::Project
{
public:
    void RegisterNodeTypes() override
    {
        auto& registry = rle::Application::Get().GetNodeRegistry();
        registry.RegisterType("PlayerNode", [](){return std::make_unique<PlayerNode>();});
    }

    std::string GetStartupScenePath() const override
    {
        return std::format(SCENE_DIR "/main.rlscene");
    }

    std::unique_ptr<rle::Scene> CreateStartupScene() override
    {
        auto scene = std::make_unique<rle::Scene>("main");
        return scene;
    }
};