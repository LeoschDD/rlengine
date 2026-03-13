#include "core/Application.hpp"

rle::Application* rle::Application::instance_ = nullptr; 

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
    SetTargetFPS(60);

    rlImGuiSetup(true);

    auto project = GetProject();

    RegisterNodeTypes();
    project->RegisterNodeTypes();
    
    if (!GetSceneManager().LoadScene(project->GetStartupScenePath()))
    {
        RLE_CORE_INFO("can't load scene - creating startup scene");
        auto scene = project->CreateStartupScene();
        if (!scene)
        {
            GetSceneManager().CreateDefaultScene();
        }
        else
        {
            GetSceneManager().SetScene(std::move(scene));
        }
    }
    return true;
}

void rle::Application::RegisterNodeTypes()
{
    node_registry_.RegisterType("Node", [](){return std::make_unique<Node>();});
    node_registry_.RegisterType("Node2D", [](){return std::make_unique<Node2D>();});
    node_registry_.RegisterType("Node3D", [](){return std::make_unique<Node3D>();});
    node_registry_.RegisterType("NodeMesh3D", [](){return std::make_unique<NodeMesh3D>();});
}

void rle::Application::Input()
{
    GetSceneManager().ProcessInput();
}

void rle::Application::Update(const float dt)
{
    GetSceneManager().ProcessUpdate(dt);
}

void rle::Application::Render()
{
    ClearBackground(SKYBLUE);
    GetSceneManager().ProcessRender();
}

rle::Application::Application()
    : scene_manager_(&node_registry_)
{
    instance_ = this;

    if (!Init())
    {
        RLE_CORE_ERROR("initialization failed");
        return;
    }
    RLE_CORE_TRACE("initialization successful");
    running_ = true;
}

rle::Application::~Application()
{
    if (auto* scene = GetSceneManager().GetScene())
    {
        GetSceneManager().SaveScene(SCENE_DIR "/" + scene->GetName() + ".rlscene");
    }
    GetSceneManager().SetScene(nullptr);
    
    CloseWindow();
    instance_ = nullptr;
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
        const float dt = GetFrameTime();

        Input();
        Update(dt);

        BeginDrawing();
        Render();
        EndDrawing();
    }
}