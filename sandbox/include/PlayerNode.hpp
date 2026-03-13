#include <rlengine.hpp>

class PlayerNode : public rle::NodeMesh3D
{
private:

public:
    PlayerNode() = default;
    
    std::string GetTypeName() const override {return "PlayerNode";}

    void OnUpdate(const float dt) override
    {
        if (IsKeyDown(KEY_W)) Translate(Vector3Scale(GetForward(), dt));
        if (IsKeyDown(KEY_S)) Translate(Vector3Scale(GetForward(), -dt));
        if (IsKeyDown(KEY_D)) Translate(Vector3Scale(GetRight(), dt));
        if (IsKeyDown(KEY_A)) Translate(Vector3Scale(GetRight(), -dt));
    }
};  