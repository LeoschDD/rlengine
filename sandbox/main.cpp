#include <rlengine.hpp>
#include "SandboxModule.hpp"

class SandboxApplication : public rle::Application
{
protected:
    void RegisterCustomNodeTypes(rle::NodeRegistry& registry) override
    {
        sandbox::RegisterNodeTypes(registry);
    }

    bool OnInit() override
    {
        GetSceneManager()->SetScene(sandbox::CreateStartupScene());
        return true;
    }

public:
    ~SandboxApplication()
    {
        GetSceneManager()->SaveScene(SANDBOX_DIR "/scene1.rlscene");
    }
};

int main()
{
    auto sandbox_app = std::make_unique<SandboxApplication>();
    sandbox_app->Run();
}