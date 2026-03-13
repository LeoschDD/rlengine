#include "core/Application.hpp"

rle::Application* rle::Application::instance_ = nullptr; 

bool rle::Application::Init()
{
    rle::Log::Init();
    if (!InitWindow()) return false;
    rlImGuiSetup(true);
    RegisterNodeTypes();
    if (!InitScene()) return false;

    return true;
}

bool rle::Application::InitWindow()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    ::InitWindow(1920, 1080, "Sandbox");
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
    return true;
}

void rle::Application::RegisterNodeTypes()
{
    node_registry_.RegisterType("Node", [](){return std::make_unique<Node>();});
    node_registry_.RegisterType("Node2D", [](){return std::make_unique<Node2D>();});
    node_registry_.RegisterType("Node3D", [](){return std::make_unique<Node3D>();});
    node_registry_.RegisterType("NodeMesh3D", [](){return std::make_unique<NodeMesh3D>();});
    node_registry_.RegisterType("NodeCamera3D", [](){return std::make_unique<NodeCamera3D>();});
    node_registry_.RegisterType("NodeCamera2D", [](){return std::make_unique<NodeCamera2D>();});

    rle::RegisterNodeTypes();
}

bool rle::Application::InitScene()
{
    if (!GetSceneManager().LoadScene(SCENE_DIR "/main.rlscene"))
    {
        RLE_CORE_INFO("can't load main scene - creating main scene");
        GetSceneManager().SetScene(std::make_unique<Scene>("main")); 
    }
    return true;
}

void rle::Application::Close()
{
    if (auto* scene = GetSceneManager().GetScene())
    {
        GetSceneManager().SaveScene(SCENE_DIR "/" + scene->GetName() + ".rlscene");
    }
    GetSceneManager().SetScene(nullptr);
    
    CloseWindow();
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
    Close();
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