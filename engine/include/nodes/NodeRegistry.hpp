#pragma once

#include "core/Headers.hpp"
#include "nodes/Node.hpp"

namespace rle
{
    class NodeRegistry
    {
    private:
        using NodeFactory = std::function<std::unique_ptr<Node>()>;
        std::unordered_map<std::string, NodeFactory> node_factories_;
    
    public:
        void RegisterType(const std::string& name, const NodeFactory& node_factory);
        std::unique_ptr<Node> CreateNode(const std::string& name) const;
        bool HasType(const std::string& name) {return node_factories_.contains(name);}
    };

    void RegisterCustomNodeTypes(NodeRegistry& node_registry);
}
