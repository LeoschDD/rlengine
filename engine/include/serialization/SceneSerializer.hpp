#pragma once

#include "core/Headers.hpp"
#include "serialization/NodeRegistry.hpp"
#include "scene/Scene.hpp"

namespace rle
{
    class SceneSerializer
    {
    private:
        NodeRegistry* node_registry_{nullptr};
    
    private:
        nlohmann::ordered_json SerializeNode(const Node* node) const;
        std::unique_ptr<Node> DeserializeNode(const nlohmann::ordered_json& json);  

    public:
        explicit SceneSerializer(NodeRegistry* node_registry) : node_registry_(node_registry) {}

        void SerializeToFile(const Scene* scene, const std::string& path) const;
        std::unique_ptr<Scene> DeserializeFromFile(const std::string& path);
    };
}