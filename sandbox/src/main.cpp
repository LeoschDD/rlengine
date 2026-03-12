#include <rlengine.hpp>
#include "SandboxProject.hpp"

rle::Project* rle::GetProject()
{
    static SandboxProject project;
    return &project;
}

int main()
{
    auto sandbox_app = std::make_unique<rle::Application>();
    sandbox_app->Run();
}