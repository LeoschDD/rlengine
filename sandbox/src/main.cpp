#include <rlengine.hpp>

class SandboxApplication : public rle::Application
{

};

class PlayerNode : public rle::Node
{

};

void rle::RegisterCustomNodeTypes(rle::NodeRegistry& node_registry)
{
    node_registry.RegisterType("PlayerNode", [](){return std::make_unique<PlayerNode>();});
}

int main()
{
    auto sandbox_app = std::make_unique<SandboxApplication>();
    sandbox_app->Run();
}