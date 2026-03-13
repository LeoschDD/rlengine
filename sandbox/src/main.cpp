#include "Sandbox.hpp"

int main()
{
    auto sandbox_app = std::make_unique<rle::Application>();
    sandbox_app->Run();
}