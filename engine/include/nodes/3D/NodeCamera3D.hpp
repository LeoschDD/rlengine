#pragma once

#include "nodes/3D/Node3D.hpp"

namespace rle
{
    class SceneManager;

    class NodeCamera3D : public rle::Node3D
    {
    private:
        friend class SceneManager;

        float fov_{60.0f};
        CameraProjection projection_{CAMERA_PERSPECTIVE};
        bool is_current_{false};

    private:
        void SetCurrent(bool current) {is_current_ = current;}

    protected:
        void EnterTreeInternal() override;
        void ExitTreeInternal() override;

    public:
        NodeCamera3D() = default;

        void Serialize(nlohmann::ordered_json& json) const override;
        void Deserialize(const nlohmann::ordered_json& json) override;
        std::string GetTypeName() const override {return "NodeCamera3D";}

        Camera3D GetCamera();
        float GetFov() const {return fov_;}
        void SetFov(float fov) {fov_ = fov;}
        CameraProjection GetProjection() const {return projection_;}
        void SetProjection(CameraProjection projection) {projection_ = projection;}
        bool IsCurrent() {return is_current_;}
    };
}