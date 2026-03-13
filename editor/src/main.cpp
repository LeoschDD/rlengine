#include <rlengine.hpp>
#include "SandboxProject.hpp"

class EditorApplication : public rle::Application
{
private:
    RenderTexture2D viewport_{};
    bool initialized_{false};
protected:
    void Render() override
    {
        if (!initialized_)
        {
            viewport_ = LoadRenderTexture(1280, 720);
            initialized_ = true;
        }
        ClearBackground(DARKGRAY);
        
        rlImGuiBegin();
        BeginTextureMode(viewport_);
        rle::Application::Render();
        EndTextureMode();
        bool open = true;

        DrawViewport();
        DrawNodeTree();
		rlImGuiEnd();
    };

    void DrawViewport()
    {
        if (ImGui::Begin("Viewport"))
        {
            ImVec2 avail = ImGui::GetContentRegionAvail();

            ImGui::Image(
                (ImTextureID)(uintptr_t)viewport_.texture.id,
                avail,
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }
        ImGui::End();
    }

    void DrawNodeTree()
    {
        bool open = true;
		if (ImGui::Begin("Nodes", &open))
		{
            DrawNodesRecursive(GetSceneManager().GetScene()->Root());
		}
        ImGui::End();
    }

    void DrawNodesRecursive(rle::Node* node)
    {
        if (!node) return;
        if (ImGui::TreeNode(node->GetName().c_str()))
        {
            if (ImGui::Button("add child"))
            {
                node->AddChild(std::make_unique<rle::Node>());
            }
            for (auto& child : node->GetChildren())
            {
                DrawNodesRecursive(child.get());
            }
            ImGui::TreePop();
        }
    }

public:
    ~EditorApplication() override
    {
        UnloadRenderTexture(viewport_);
    }
};

rle::Project* rle::GetProject()
{
    static SandboxProject project;
    return &project;
}

int main()
{
    auto editor_app = std::make_unique<EditorApplication>();
    editor_app->Run();
}