#pragma once

#include "serialization/NodeRegistry.hpp"
#include "scene/Scene.hpp"

namespace rle
{
    class Project
    {
    public:
        virtual ~Project() = default;

        virtual std::unique_ptr<rle::Scene> CreateStartupScene() = 0;
        virtual std::string GetStartupScenePath() const = 0;
        virtual void RegisterNodeTypes(rle::NodeRegistry& node_registry) = 0;
    };

    Project* GetProject();
}