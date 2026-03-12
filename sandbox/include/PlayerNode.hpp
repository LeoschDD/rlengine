#include <rlengine.hpp>

class PlayerNode : public rle::Node
{
private:
    Vector3 position {0.0f, 0.0f, 0.0f};

public:
    PlayerNode() : rle::Node("Player") {}
    void Serialize(nlohmann::ordered_json& json) const override
    {
        rle::Node::Serialize(json);
        json["position"] = {position.x, position.y, position.z};
    }
    void Deserialize(const nlohmann::ordered_json& json) override
    {
        rle::Node::Deserialize(json);
        position.x = json["position"][0];
        position.y = json["position"][1];
        position.z = json["position"][2];
    }
    const std::string GetTypeName() const override {return "PlayerNode";}
};  