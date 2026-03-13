#include "nodes/2D/Node2D.hpp"
#include "core/Log.hpp"

void rle::Node2D::MarkTransformDirty()
{
    transform_dirty_ = true;

    for (const auto& c : GetChildren())
    {
        if (auto* child = dynamic_cast<Node2D*>(c.get()))
        {
            child->MarkTransformDirty();
        }
    }
    OnTransformChanged();
}

void rle::Node2D::UpdateGlobalTransform()
{
    if (!transform_dirty_) return;

    if (InheritsParentTransform())
    {
        if (Node2D* parent = dynamic_cast<Node2D*>(GetParent()))
        {
            parent->UpdateGlobalTransform();

            Vector2 scaled_pos = Vector2Multiply(local_transform_.translation, parent->global_transform_.scale);
            float cos = std::cos(parent->global_transform_.rotation);
            float sin = std::sin(parent->global_transform_.rotation);
            global_transform_.translation.x = parent->global_transform_.translation.x + scaled_pos.x * cos - scaled_pos.y * sin;
            global_transform_.translation.y = parent->global_transform_.translation.y + scaled_pos.x * sin + scaled_pos.y * cos;
            global_transform_.rotation = parent->global_transform_.rotation + local_transform_.rotation;
            global_transform_.scale = Vector2Multiply(parent->global_transform_.scale, local_transform_.scale);
        }
        else
        {
            global_transform_ = local_transform_;
        }
    }
    else
    {
        global_transform_ = local_transform_;
    }
    transform_dirty_ = false;
}

void rle::Node2D::Serialize(nlohmann::ordered_json &json) const
{
    Node::Serialize(json);

    Vector2 pos = local_transform_.translation;
    json["position"] = {pos.x, pos.y};

    float rotation = local_transform_.rotation;
    json["rotation"] = rotation;

    Vector2 scale = local_transform_.scale;
    json["scale"] = {scale.x, scale.y};
}

void rle::Node2D::Deserialize(const nlohmann::ordered_json &json)
{
    Node::Deserialize(json);

    if (json.contains("position"))
    {
        auto pos = json["position"];
        if (pos.size() == 2)
        {
            local_transform_.translation = {pos[0], pos[1]};        
        }        
    }
    else
    {
        RLE_CORE_WARN("failed to load node - position not found");
    }
    if (json.contains("rotation"))
    {
        local_transform_.rotation = json["rotation"].get<float>();
    }
    else
    {
        RLE_CORE_WARN("failed to load node - rotation not found");
    }
    if (json.contains("scale"))
    {
        auto scale = json["scale"];
        if (scale.size() == 2)
        {
            local_transform_.scale = {scale[0], scale[1]};
        }
    }
    else
    {
        RLE_CORE_WARN("failed to load node - scale not found");
    }
}

void rle::Node2D::Translate(Vector2 offset)
{
    local_transform_.translation = Vector2Add(local_transform_.translation, offset);
    MarkTransformDirty();
}

void rle::Node2D::Rotate(float angle)
{
    local_transform_.rotation += angle;
    MarkTransformDirty();
}

rle::Transform2D rle::Node2D::GetGlobalTransform()
{
    UpdateGlobalTransform();
    return global_transform_;
}

Vector2 rle::Node2D::GetRight()
{
    float rot = GetGlobalTransform().rotation;
    return {std::cos(rot), std::sin(rot)};
}

Vector2 rle::Node2D::GetUp()
{
    float rot = GetGlobalTransform().rotation;
    return {std::sin(rot), -std::cos(rot)};
}
