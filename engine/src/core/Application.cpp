#include "core/Application.hpp"

bool rle::Application::Init()
{
    rle::Log::Init();
    RLE_CORE_TRACE("initialized log");

    InitWindow(1920, 1080, "Sandbox");
    if (!IsWindowReady()) 
    {
        RLE_CORE_ERROR("window could not initialize");
        return false;
    }
    else 
    {
        RLE_CORE_TRACE("initialized window");
    }
    RegisterNodeTypes();
    return OnInit();
}

void rle::Application::RegisterNodeTypes()
{
    node_registry_.RegisterType("Node", [](){return std::make_unique<Node>();});
    RegisterCustomNodeTypes(node_registry_);
}

rle::Application::Application()
{
    if (!Init()) 
    {
        RLE_CORE_ERROR("initialization failed");
    }
    else
    {
        RLE_CORE_TRACE("initialization successful");
        running_ = true;
    }
}

void rle::Application::Run()
{
    while (running_)
    {
        if (WindowShouldClose()) 
        {
            running_ = false;
            break;
        }
        BeginDrawing();
        ClearBackground(WHITE);
        EndDrawing();
    }
}