#include "nodes/3D/Node3D.hpp"

void rle::Node3D::MarkTransformDirty()
{
    transform_dirty_ = true;

    for (const auto& c : GetChildren())
    {
        if (auto* child = dynamic_cast<Node3D*>(c.get()))
        {
            child->MarkTransformDirty();
        }
    }
    OnTransformChanged();
}

void rle::Node3D::UpdateGlobalTransform() 
{
    if (!transform_dirty_) return;

    if (InheritsParentTransform())
    {
        if (Node3D* parent = dynamic_cast<Node3D*>(GetParent()))
        {
            parent->UpdateGlobalTransform();

            Vector3 scaled_position = Vector3Multiply(local_transform_.translation, parent->global_transform_.scale);
            global_transform_.translation = Vector3Add(Vector3RotateByQuaternion(scaled_position, parent->global_transform_.rotation), parent->global_transform_.translation);
            global_transform_.rotation = QuaternionMultiply(parent->global_transform_.rotation, local_transform_.rotation);
            global_transform_.scale = Vector3Multiply(parent->global_transform_.scale, local_transform_.scale);
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
    global_transform_.rotation = QuaternionNormalize(global_transform_.rotation);
    transform_dirty_ = false;
}

void rle::Node3D::Serialize(nlohmann::ordered_json &json) const
{
    Vector3 pos = local_transform_.translation;
    json["postion"] = {pos.x, pos.y, pos.z};

    Quaternion rotation = local_transform_.rotation;
    json["rotation"] = {rotation.x, rotation.y, rotation.z, rotation.w};

    Vector3 scale = local_transform_.scale;
    json["scale"] = {scale.x, scale.y, scale.z};
}

void rle::Node3D::Deserialize(const nlohmann::ordered_json &json)
{
    auto pos = json["position"];
    if (pos.size() == 3)
    {
        local_transform_.translation = {pos[0], pos[1], pos[2]};        
    }
    auto rotation = json["rotation"];
    if (rotation.size() == 4)
    {
        local_transform_.rotation = {rotation[0], rotation[1], rotation[2], rotation[3]};        
    }
    auto scale = json["scale"];
    if (scale.size() == 3)
    {
        local_transform_.scale = {scale[0], scale[1], scale[2]};
    }
}

void rle::Node3D::Translate(Vector3 offset)
{
    local_transform_.translation = Vector3Add(local_transform_.translation, offset);
    MarkTransformDirty();
}

void rle::Node3D::Rotate(Quaternion rotation)
{
    local_transform_.rotation = QuaternionNormalize(QuaternionMultiply(rotation, local_transform_.rotation));
    MarkTransformDirty();
}

void rle::Node3D::Rotate(Vector3 axis, float angle)
{
    Rotate(QuaternionFromAxisAngle(axis, angle));
}

void rle::Node3D::RotateLocal(Quaternion rotation)
{
    local_transform_.rotation = QuaternionNormalize(QuaternionMultiply(local_transform_.rotation, rotation));
    MarkTransformDirty();
}

void rle::Node3D::RotateLocal(Vector3 axis, float angle)
{
    RotateLocal(QuaternionFromAxisAngle(axis, angle));
}

void rle::Node3D::LookAt(Vector3 target, Vector3 up)
{
    Vector3 global_pos = GetGlobalTransform().translation;

    Vector3 dir = Vector3Subtract(target, global_pos);
    if (Vector3Length(dir) < 0.0001f) return;
    dir = Vector3Normalize(dir);

    Vector3 back = Vector3Negate(dir);
    Vector3 right = Vector3CrossProduct(up, back);
    if (Vector3Length(right) < 0.0001f) return;
    right = Vector3Normalize(right);
    Vector3 orthogonal_up = Vector3CrossProduct(back, right);

    Matrix m = {
        right.x, orthogonal_up.x, back.x, 0,
        right.y, orthogonal_up.y, back.y, 0,
        right.z, orthogonal_up.z, back.z, 0,
        0, 0, 0, 1
    };
    Quaternion world_rotation = QuaternionFromMatrix(m);

    if (InheritsParentTransform())
    {
        if (Node3D* parent = dynamic_cast<Node3D*>(GetParent()))
        {
            Quaternion parent_inv = QuaternionInvert(parent->GetGlobalTransform().rotation);
            world_rotation = QuaternionMultiply(parent_inv, world_rotation);
        }
    }

    local_transform_.rotation = world_rotation;
    MarkTransformDirty();
}

Transform rle::Node3D::GetGlobalTransform()
{
    UpdateGlobalTransform();
    return global_transform_;
}

Matrix rle::Node3D::GetGlobalTransformMatrix()
{
    UpdateGlobalTransform();

    Matrix transform = MatrixIdentity();
    transform = MatrixMultiply(transform, MatrixScale(global_transform_.scale.x, global_transform_.scale.y, global_transform_.scale.z));
    transform = MatrixMultiply(transform, QuaternionToMatrix(global_transform_.rotation));
    transform = MatrixMultiply(transform, MatrixTranslate(global_transform_.translation.x, global_transform_.translation.y, global_transform_.translation.z));
    return transform;
}

Vector3 rle::Node3D::GetForward()
{
    return Vector3RotateByQuaternion({0.0f, 0.0f, -1.0f}, GetGlobalTransform().rotation);
}

Vector3 rle::Node3D::GetRight()
{
    return Vector3RotateByQuaternion({1.0f, 0.0f, 0.0f}, GetGlobalTransform().rotation);
}

Vector3 rle::Node3D::GetUp()
{
    return Vector3RotateByQuaternion({0.0f, 1.0f, 0.0f}, GetGlobalTransform().rotation);
}