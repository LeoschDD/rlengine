#include "scene/Scene.hpp"

uint32_t rle::Scene::next_id_ = 0;

rle::Scene::Scene()
    : root_(std::make_unique<Node>("Root"))
{
    name_ = std::format("Scene_{}", next_id_++);
}

rle::Scene::Scene(std::unique_ptr<Node> root)
    : root_(std::move(root))
{
    name_ = std::format("Scene_{}", next_id_++);
}

rle::Scene::Scene(const std::string& name)
    : root_(std::make_unique<Node>("Root"))
{
    name_ = name;
}

rle::Scene::Scene(std::unique_ptr<Node> root, const std::string& name)
    : root_(std::move(root))
{
    name_ = name;
}