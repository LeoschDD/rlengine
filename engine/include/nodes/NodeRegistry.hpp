#pragma once

#include "core/Headers.hpp"
#include "nodes/Node.hpp"

namespace rle
{
    class NodeRegistry
    {
    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<Node>()>> node_factories_;
    
    public:
        void Register(const std::string& name, const std::function<std::unique_ptr<Node>>& node_factory);
        std::unique_ptr<Node> CreateNode(const std::string& name) const;
    };
}
