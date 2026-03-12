#pragma once

#include "serialization/NodeRegistry.hpp"
#include "scene/Scene.hpp"

namespace rle
{
    class Project
    {
    public:
        virtual std::unique_ptr<rle::Scene> CreateStartupScene() = 0;
        virtual const std::string GetStartupScenePath() const = 0;
        virtual void RegisterNodeTypes(rle::NodeRegistry& node_registry) = 0;
    };

    Project* GetProject();
}