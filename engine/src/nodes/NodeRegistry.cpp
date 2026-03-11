#include "nodes/NodeRegistry.hpp"
#include "core/Log.hpp"

void rle::NodeRegistry::Register(const std::string& name, const std::function<std::unique_ptr<Node>>& node_factory)
{
    auto it = node_factories_.find(name);
    if (it != node_factories_.end())
    {
        RLE_CORE_WARN("already registered node factory with the name {}", name);
        return;
    }
    node_factories_.emplace(name, std::move(node_factory));
}

std::unique_ptr<rle::Node> rle::NodeRegistry::CreateNode(const std::string& name) const
{
    auto it = node_factories_.find(name);
    if (it == node_factories_.end())
    {
        RLE_CORE_WARN("could not find node factory with the name {}", name);
        return nullptr;
    }
    return it->second();
}