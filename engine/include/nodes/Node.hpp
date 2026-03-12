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

        // std::vector<std::function<void>> queued_operations_
    
    private:
        // EnterTree
        // ExitTree
        // Ready

        // Input
        // Update
        // Render
    
    protected:        
        // OnEnterTree
        // ...

    public:
        Node();
        Node(const std::string& name);
        virtual ~Node() = default;

        virtual void Serialize(nlohmann::ordered_json& json) const;
        virtual void Deserialize(const nlohmann::ordered_json& json);

        void AddChild(std::unique_ptr<Node> child);
        // RemoveChild
        // DetachChild
        // Reparent

        // AddChildQueued
        // RemoveChildQueued
        // DetachChildQueued
        // ReparentQueued

        // QueueFree

        // node* GetChild(id)
        // node* GetChild(name)

        uint32_t GetId() const {return id_;}
        const std::string& GetName() const {return name_;}
        virtual std::string GetTypeName() const {return "Node";}

        Node* GetParent() {return parent_;}
        const Node* GetParent() const {return parent_;}
        std::vector<std::unique_ptr<Node>>& GetChildren() {return children_;}
        const std::vector<std::unique_ptr<Node>>& GetChildren() const {return children_;}
    };
}
