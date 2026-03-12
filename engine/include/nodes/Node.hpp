#pragma once

#include "core/Headers.hpp"

namespace rle
{
    class SceneManager;

    class Node
    {
    private:
        friend class SceneManager;

        static uint32_t next_id_;
        uint32_t id_;
        std::string name_;

        bool active_{true};
        bool in_tree_{false};
        bool ready_{false};
        bool processing_{false};

        Node* parent_{nullptr};
        std::vector<std::unique_ptr<Node>> children_;

        std::vector<std::function<void()>> queued_operations_;
    
    private:
        void EnterTree();
        void ExitTree();
        void Input();
        void Update(const float dt);
        void Render();

        [[nodiscard]] bool HasAncestor(const Node* ancestor) const;
        void FlushQueuedOperations();
    
    protected:        
        virtual void EnterTreeInternal() {}
        virtual void ExitTreeInternal() {}
        virtual void ReadyInternal() {}
        virtual void InputInternal() {}
        virtual void UpdateInternal(const float dt) {}
        virtual void RenderInternal() {}

        virtual void OnEnterTree() {}
        virtual void OnExitTree() {}
        virtual void OnReady() {}
        virtual void OnInput() {}
        virtual void OnUpdate(const float dt) {}
        virtual void OnRender() {}
        
    public:
        Node();
        Node(const std::string& name);
        virtual ~Node() = default;

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) = delete;

        virtual void Serialize(nlohmann::ordered_json& json) const;
        virtual void Deserialize(const nlohmann::ordered_json& json);

        void AddChild(std::unique_ptr<Node> child);
        void RemoveChild(Node* child);
        void Reparent(Node* parent);
        std::unique_ptr<Node> DetatchChild(Node* child);

        void AddChildQueued(std::unique_ptr<Node> child);
        void RemoveChildQueued(Node* child);
        void ReparentQueued(Node* parent);

        Node* GetChild(const std::string& name);
        Node* GetChild(uint32_t id);
        Node* GetChildRecursive(const std::string& name);
        Node* GetChildRecursive(uint32_t id);

        uint32_t GetId() const {return id_;}
        const std::string& GetName() const {return name_;}
        virtual std::string GetTypeName() const {return "Node";}

        Node* GetParent() {return parent_;}
        const Node* GetParent() const {return parent_;}
        std::vector<std::unique_ptr<Node>>& GetChildren() {return children_;}
        const std::vector<std::unique_ptr<Node>>& GetChildren() const {return children_;}
    };
}
