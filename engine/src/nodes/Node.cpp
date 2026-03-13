#include "nodes/Node.hpp"
#include "core/Log.hpp"

uint32_t rle::Node::next_id_ = 0;

rle::Node::Node()
    : id_(next_id_++)
{
    name_ = std::format("Node_{}", id_);
}

rle::Node::Node(const std::string& name)
    : id_(next_id_++)
{
    name_ = name;
}

void rle::Node::EnterTree()
{
    in_tree_ = true;
    processing_ = true;

    EnterTreeInternal();
    OnEnterTree();

    for (auto& child : children_)
    {
        child->EnterTree();
    }

    processing_ = false;
    FlushQueuedOperations();

    processing_ = true;
    if (!ready_)
    {
        ready_ = true;
        ReadyInternal();
        OnReady();
    }

    processing_ = false;
    FlushQueuedOperations();
}

void rle::Node::ExitTree()
{
    processing_ = true;

    for (auto& child : children_)
    {
        child->ExitTree();
    }

    OnExitTree();
    ExitTreeInternal();

    processing_ = false;
    FlushQueuedOperations();

    in_tree_ = false;
}

void rle::Node::Input()
{
    if (!active_) return;

    processing_ = true;

    InputInternal();
    OnInput();

    for (auto& child : children_)
    {
        child->Input();
    }

    processing_ = false;
    FlushQueuedOperations();
}

void rle::Node::Update(const float dt)
{
    if (!active_) return;

    processing_ = true;

    UpdateInternal(dt);
    OnUpdate(dt);

    for (auto& child : children_)
    {
        child->Update(dt);
    }

    processing_ = false;
    FlushQueuedOperations();
}

void rle::Node::Render()
{
    if (!active_) return;

    processing_ = true;

    RenderInternal();
    OnRender();

    for (auto& child : children_)
    {
        child->Render();
    }

    processing_ = false;
    FlushQueuedOperations();
}

void rle::Node::Render2D()
{
    if (!active_) return;

    processing_ = true;

    Render2DInternal();
    OnRender2D();

    for (auto& child : children_)
    {
        child->Render2D();
    }

    processing_ = false;
    FlushQueuedOperations();
}

void rle::Node::Render3D()
{
    if (!active_) return;

    processing_ = true;

    Render3DInternal();
    OnRender3D();

    for (auto& child : children_)
    {
        child->Render3D();
    }

    processing_ = false;
    FlushQueuedOperations();
}

bool rle::Node::HasAncestor(const Node* ancestor) const
{
    const Node* current = parent_;
    while (current)
    {
        if (current == ancestor) return true;
        current = current->parent_;
    }
    return false;
}

void rle::Node::FlushQueuedOperations()
{
    if (queued_operations_.empty()) return;

    auto ops = std::move(queued_operations_);
    queued_operations_.clear();

    for (auto& op : ops)
    {
        op();
    }
}

void rle::Node::Serialize(nlohmann::ordered_json& json) const
{
    json["type"] = GetTypeName();
    json["id"] = id_;
    json["name"] = name_;
}

void rle::Node::Deserialize(const nlohmann::ordered_json& json)
{
    if (json.contains("id"))
    {
        id_ = json["id"].get<uint32_t>();
    }
    else
    {
        RLE_CORE_ERROR("failed to load node - id not found");
    }
    if (json.contains("name"))
    {
        name_ = json["name"].get<std::string>();
    }
    else
    {
        RLE_CORE_ERROR("failed to load node - name not found");
    }
    next_id_ = std::max(id_ + 1, next_id_);
}

void rle::Node::AddChild(std::unique_ptr<Node> child)
{
    if (!child) return;
    if (this == child.get())
    {
        RLE_CORE_WARN("cannot add '{}' as child of itself", name_);
        return;
    }
    if (HasAncestor(child.get()))
    {
        RLE_CORE_WARN("cannot add ancestor '{}' as child of '{}'", child->GetName(), name_);
        return;
    }
    if (processing_)
    {
        AddChildQueued(std::move(child));
        return;
    }
    child->parent_ = this;
    Node* raw = child.get();
    children_.push_back(std::move(child));

    if (in_tree_)
    {
        raw->EnterTree();
    }
}

void rle::Node::RemoveChild(Node* child)
{
    if (!child) return;
    if (processing_)
    {
        RemoveChildQueued(child);
        return;
    }

    auto it = std::find_if(children_.begin(), children_.end(),
        [child](const std::unique_ptr<Node>& c) { return c.get() == child; });

    if (it == children_.end())
    {
        RLE_CORE_WARN("node '{}' is not a child of '{}'", child->GetName(), name_);
        return;
    }

    if (child->in_tree_)
    {
        child->ExitTree();
    }

    children_.erase(it);
}

void rle::Node::Reparent(Node* new_parent)
{
    if (!new_parent)
    {
        RLE_CORE_WARN("new parent is null for node '{}'", name_);
        return;
    }
    if (new_parent == parent_) return;
    if (new_parent == this)
    {
        RLE_CORE_WARN("cannot reparent '{}' to itself", name_);
        return;
    }
    if (new_parent->HasAncestor(this))
    {
        RLE_CORE_WARN("cannot reparent '{}' to descendant '{}'", name_, new_parent->GetName());
        return;
    }

    if (parent_ && parent_->processing_)
    {
        parent_->queued_operations_.push_back([this, new_parent]() {
            Reparent(new_parent);
        });
        return;
    }
    if (new_parent->processing_)
    {
        new_parent->queued_operations_.push_back([this, new_parent]() {
            Reparent(new_parent);
        });
        return;
    }

    std::unique_ptr<Node> self;
    if (parent_)
    {
        self = parent_->DetatchChild(this);
    }

    if (self)
    {
        new_parent->AddChild(std::move(self));
    }
}

std::unique_ptr<rle::Node> rle::Node::DetatchChild(Node* child)
{
    if (!child) return nullptr;
    if (processing_)
    {
        RLE_CORE_WARN("'{}' is processing, use Reparent instead", name_);
        return nullptr;
    }

    auto it = std::find_if(children_.begin(), children_.end(),
        [child](const std::unique_ptr<Node>& c) { return c.get() == child; });

    if (it == children_.end())
    {
        RLE_CORE_WARN("node '{}' is not a child of '{}'", child->GetName(), name_);
        return nullptr;
    }

    if (child->in_tree_)
    {
        child->ExitTree();
    }

    std::unique_ptr<Node> detached = std::move(*it);
    children_.erase(it);
    detached->parent_ = nullptr;
    return detached;
}

void rle::Node::AddChildQueued(std::unique_ptr<Node> child)
{
    auto shared_child = std::make_shared<std::unique_ptr<Node>>(std::move(child));
    queued_operations_.push_back([this, shared_child]() {
        AddChild(std::move(*shared_child));
    });
}

void rle::Node::RemoveChildQueued(Node* child)
{
    queued_operations_.push_back([this, child]() {
        RemoveChild(child);
    });
}

void rle::Node::ReparentQueued(Node* new_parent)
{
    queued_operations_.push_back([this, new_parent]() {
        Reparent(new_parent);
    });
}

rle::Node* rle::Node::GetChild(const std::string& name)
{
    for (auto& child : children_)
    {
        if (child->name_ == name) return child.get();
    }
    return nullptr;
}

rle::Node* rle::Node::GetChild(uint32_t id)
{
    for (auto& child : children_)
    {
        if (child->id_ == id) return child.get();
    }
    return nullptr;
}

rle::Node* rle::Node::GetChildRecursive(const std::string& name)
{
    for (auto& child : children_)
    {
        if (child->name_ == name) return child.get();
        Node* found = child->GetChildRecursive(name);
        if (found) return found;
    }
    return nullptr;
}

rle::Node* rle::Node::GetChildRecursive(uint32_t id)
{
    for (auto& child : children_)
    {
        if (child->id_ == id) return child.get();
        Node* found = child->GetChildRecursive(id);
        if (found) return found;
    }
    return nullptr;
}
