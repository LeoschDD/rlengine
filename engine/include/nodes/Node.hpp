#pragma once

#include "core/Headers.hpp"

namespace rle
{
    class Node
    {
    private:
        static uint32_t next_id_;
        uint32_t id_;
        std::string name_;

        Node* parent_{nullptr};
        std::vector<std::unique_ptr<Node>> children_;
    
    public:
        Node();
        Node(const std::string& name);

        virtual void Serialize(nlohmann::ordered_json& json) const;
        virtual void Deserialize(const nlohmann::ordered_json& json);

        void AddChild(std::unique_ptr<Node> child);

        const Node* GetParent() {return parent_;}
        const std::vector<std::unique_ptr<Node>>& GetChildren() const {return children_;}
        const uint32_t GetId() {return id_;}
        const std::string& GetName() {return name_;}
        virtual const std::string GetTypeName() const {return "Node";}
    };
}
