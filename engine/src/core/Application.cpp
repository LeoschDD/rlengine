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
    project->RegisterNodeTypes(node_registry_);
    
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
}

rle::Application::Application()
    : scene_manager_(&node_registry_)
{
    instance_ = this;
}

rle::Application::~Application()
{
    if (auto* scene = GetSceneManager().GetScene())
    {
        GetSceneManager().SaveScene(SCENE_DIR "/" + scene->GetName() + ".rlscene");
    }
    CloseWindow();
    instance_ = nullptr;
}

void rle::Application::Run()
{
    if (!running_)
    {
        if (!Init())
        {
            RLE_CORE_ERROR("initialization failed");
            return;
        }
        RLE_CORE_TRACE("initialization successful");
        running_ = true;
    }

    while (running_)
    {
        if (WindowShouldClose()) 
        {
            running_ = false;
            break;
        }
        const float dt = GetFrameTime();

        GetSceneManager().ProcessInput();
        GetSceneManager().ProcessUpdate(dt);

        BeginDrawing();
        ClearBackground(SKYBLUE);

        ProcessEditor();
        GetSceneManager().ProcessRender();
         
        EndDrawing();
    }
}