#include <rlengine.hpp>
#include "PlayerNode.hpp"

void rle::RegisterNodeTypes()
{
    auto& registry = rle::Application::Get().GetNodeRegistry();
    registry.RegisterType("PlayerNode", [](){return std::make_unique<PlayerNode>();});
}