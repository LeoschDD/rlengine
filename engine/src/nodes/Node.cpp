#include "nodes/Node.hpp"

uint32_t rle::Node::next_id_ = 0;

rle::Node::Node()
    : id_(next_id_++)
{
    name_ = std::format("{}_{}", GetTypeName(), id_);
}

rle::Node::Node(const std::string& name)
    : id_(next_id_++)
{
    name_ = name;
}

void rle::Node::Serialize(nlohmann::ordered_json& json) const
{
    json["type"] = GetTypeName();
    json["id"] = id_;
    json["name"] = name_;
}

void rle::Node::Deserialize(const nlohmann::ordered_json& json)
{
    id_ = json["id"].get<uint32_t>();
    name_ = json["name"].get<std::string>();
    next_id_ = std::max(id_ + 1, next_id_);
}

void rle::Node::AddChild(std::unique_ptr<Node> child)
{
    child->parent_ = this;
    children_.push_back(std::move(child));
}