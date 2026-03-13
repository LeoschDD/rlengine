#pragma once

#include "nodes/2D/Node2D.hpp"

namespace rle
{
    class SceneManager;

    class NodeCamera2D : public rle::Node2D
    {
    private:
        friend class SceneManager;

        float zoom_{1.0f};
        bool is_current_{false};

    private:
        void SetCurrent(bool current) {is_current_ = current;}

    protected:
        void EnterTreeInternal() override;
        void ExitTreeInternal() override;

    public:
        NodeCamera2D() = default;

        void Serialize(nlohmann::ordered_json& json) const override;
        void Deserialize(const nlohmann::ordered_json& json) override;
        std::string GetTypeName() const override {return "NodeCamera2D";}

        Camera2D GetCamera();
        float GetZoom() const {return zoom_;}
        void SetZoom(float zoom) {zoom_ = zoom;}
        bool IsCurrent() {return is_current_;}
    };
}