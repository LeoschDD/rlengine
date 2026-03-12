#include "serialization/SceneSerializer.hpp"
#include "core/Log.hpp"

nlohmann::ordered_json rle::SceneSerializer::SerializeNode(const Node* node) const
{
    nlohmann::ordered_json json;
    node->Serialize(json);

    json["children"] = nlohmann::json::array();
    for (auto& child : node->GetChildren())
    {
        json["children"].push_back(SerializeNode(child.get()));    
    }
    return json;
}

std::unique_ptr<rle::Node> rle::SceneSerializer::DeserializeNode(const nlohmann::ordered_json& json)
{
    const std::string type = json["type"].get<std::string>();
    std::unique_ptr<Node> node = node_registry_->CreateNode(type);
    if (!node) 
    {
        RLE_CORE_WARN("failed to deserialize node");
        return nullptr;
    }
    node->Deserialize(json);

    if (json.contains("children"))
    {
        for (const auto& child : json["children"])
        {
            std::unique_ptr<Node> child_node = DeserializeNode(child);
            if (child_node) node->AddChild(std::move(child_node));
        }
    }
    return node;
}

void rle::SceneSerializer::SerializeToFile(const Scene* scene, const std::string& path) const
{
    nlohmann::ordered_json json;
    json["root"] = SerializeNode(scene->Root());
    std::ofstream os(path);

    if (!os.is_open())
    {
        RLE_CORE_ERROR("could not open file: {}", path);
        return;
    }
    os << json.dump(4);
    os.close();
}

std::unique_ptr<rle::Scene> rle::SceneSerializer::DeserializeFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        RLE_CORE_ERROR("could not open file: {}", path);
        return nullptr;
    }
    nlohmann::ordered_json json = nlohmann::json::parse(file);
    if (!json.contains("root"))
    {
        RLE_CORE_ERROR("cant load scene without root", path);
        return nullptr;
    }
    std::unique_ptr<Node> root = DeserializeNode(json["root"]);
    if (!root)
    {
        RLE_CORE_ERROR("failed to deserialize root");
        return nullptr;
    }
    return std::make_unique<Scene>(std::move(root));
}