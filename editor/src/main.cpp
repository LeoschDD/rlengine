#include <rlengine.hpp>
#include "SandboxProject.hpp"

class EditorApplication : public rle::Application
{
protected:
    void ProcessEditor() override
    {
        rlImGuiBegin();


        DrawNodeTree();
		rlImGuiEnd();
    };

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