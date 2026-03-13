#include "nodes/2D/NodeCamera2D.hpp"
#include "core/Log.hpp"
#include "core/Application.hpp"

void rle::NodeCamera2D::EnterTreeInternal()
{
    if (is_current_) rle::Application::Get().GetSceneManager().SetActiveCamera(this);
}

void rle::NodeCamera2D::ExitTreeInternal()
{
    if (is_current_) rle::Application::Get().GetSceneManager().SetActiveCamera(nullptr);
}

void rle::NodeCamera2D::Serialize(nlohmann::ordered_json &json) const

{
    Node2D::Serialize(json);

    json["zoom"] = zoom_;
    json["is_current"] = is_current_;
}

void rle::NodeCamera2D::Deserialize(const nlohmann::ordered_json& json)
{
    Node2D::Deserialize(json);

    if (json.contains("zoom"))
    {
        zoom_ = json["zoom"].get<float>();
    }
    else
    {
        RLE_CORE_WARN("failed to load node - zoom not found");
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

Camera2D rle::NodeCamera2D::GetCamera()
{
    return Camera2D {
        .offset = {GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f},
        .target = GetGlobalTransform().translation,
        .rotation = GetGlobalTransform().rotation,
        .zoom = zoom_
    };
}

