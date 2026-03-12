#include "scene/Scene.hpp"

rle::Scene::Scene()
    : root_(std::make_unique<Node>("Root"))
{}

rle::Scene::Scene(std::unique_ptr<Node> root)
    : root_(std::move(root))
{}