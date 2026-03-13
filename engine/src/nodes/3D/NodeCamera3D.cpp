#include "nodes/3D/NodeCamera3D.hpp"
#include "core/Log.hpp"
#include "core/Application.hpp"

void rle::NodeCamera3D::EnterTreeInternal()
{
    if (is_current_) rle::Application::Get().GetSceneManager().SetActiveCamera(this);
}

void rle::NodeCamera3D::ExitTreeInternal()
{
    if (is_current_) rle::Application::Get().GetSceneManager().SetActiveCamera(nullptr);
}

void rle::NodeCamera3D::Serialize(nlohmann::ordered_json &json) const

{
    Node3D::Serialize(json);

    json["fov"] = fov_;
    json["projection"] = projection_;
    json["is_current"] = is_current_;
}

void rle::NodeCamera3D::Deserialize(const nlohmann::ordered_json& json)
{
    Node3D::Deserialize(json);

    if (json.contains("fov"))
    {
        fov_ = json["fov"].get<float>();
    }
    else
    {
        RLE_CORE_WARN("failed to load node - fov not found");
    }
    if (json.contains("projection"))
    {
        projection_ = json["projection"].get<CameraProjection>();
    }
    else
    {
        RLE_CORE_WARN("failed to load node - projection not found");
    }
    if (json.contains("is_current"))
    {
        is_current_ = json["is_current"].get<bool>();
    }
    else
    {
        RLE_CORE_WARN("failed to load node - is_current not found");
    }
}

Camera3D rle::NodeCamera3D::GetCamera()
{
    Vector3 target = Vector3Add(GetGlobalTransform().translation, GetForward());

    return Camera3D {
        .position = GetGlobalTransform().translation,
        .target = target,
        .up = GetUp(),
        .fovy = fov_,
        .projection = projection_
    };
}

