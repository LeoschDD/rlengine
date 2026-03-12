#pragma once

#include <rlengine.hpp>

namespace sandbox
{
    void RegisterNodeTypes(rle::NodeRegistry& registry);
    std::unique_ptr<rle::Scene> CreateStartupScene();
}