#include <rlengine.hpp>

class SandboxApplication : public rle::Application
{

};

int main()
{
    auto sandbox_app = std::make_unique<SandboxApplication>();
    sandbox_app->Run();
}