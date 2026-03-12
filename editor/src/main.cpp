#include <rlengine.hpp>
#include "SandboxModule.hpp"

class EditorApplication : public rle::Application
{
protected:
    void RegisterCustomNodeTypes(rle::NodeRegistry& registry) override
    {
        sandbox::RegisterNodeTypes(registry);
    }

    bool OnInit() override
    {
        GetSceneManager().SetScene(sandbox::CreateStartupScene());
        return true;
    }
public:
    ~EditorApplication()
    {
        GetSceneManager().SaveScene(SANDBOX_DIR "/scene1.rlscene");
    }
};

int main()
{
    auto editor_app = std::make_unique<EditorApplication>();
    editor_app->Run();
}