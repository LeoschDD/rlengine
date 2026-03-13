#include <rlengine.hpp>
#include "SandboxProject.hpp"

class EditorApplication : public rle::Application
{
private:
    RenderTexture2D viewport_{};
    bool initialized_{false};
    rle::Node* selected_node_{nullptr};

    // Per-node add-child state keyed by node ID
    std::unordered_map<uint32_t, std::string> add_child_type_;
    std::unordered_map<uint32_t, std::string> add_child_name_;

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

        DrawViewport();
        DrawNodeTree();
        DrawInspector();
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

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (node == selected_node_)
            flags |= ImGuiTreeNodeFlags_Selected;
        if (node->GetChildren().empty())
            flags |= ImGuiTreeNodeFlags_Leaf;

        bool opened = ImGui::TreeNodeEx((void*)(intptr_t)node->GetId(), flags, "%s", node->GetName().c_str());

        if (ImGui::IsItemClicked())
            selected_node_ = node;

        if (opened)
        {
            uint32_t nid = node->GetId();

            // Ensure buffers exist for this node
            if (add_child_type_.find(nid) == add_child_type_.end())
                add_child_type_[nid] = "Node";
            if (add_child_name_.find(nid) == add_child_name_.end())
                add_child_name_[nid] = "";

            ImGui::PushID(nid);

            // Type input
            ImGui::SetNextItemWidth(100);
            char type_buf[128];
            strncpy(type_buf, add_child_type_[nid].c_str(), sizeof(type_buf) - 1);
            type_buf[sizeof(type_buf) - 1] = '\0';
            if (ImGui::InputText("Type", type_buf, sizeof(type_buf)))
                add_child_type_[nid] = type_buf;

            ImGui::SameLine();

            // Name input
            ImGui::SetNextItemWidth(100);
            char name_buf[128];
            strncpy(name_buf, add_child_name_[nid].c_str(), sizeof(name_buf) - 1);
            name_buf[sizeof(name_buf) - 1] = '\0';
            if (ImGui::InputText("Name", name_buf, sizeof(name_buf)))
                add_child_name_[nid] = name_buf;

            ImGui::SameLine();

            // Add child button
            if (ImGui::Button("Add Child"))
            {
                auto& registry = GetNodeRegistry();
                const std::string& type = add_child_type_[nid];
                if (registry.HasType(type))
                {
                    auto child = registry.CreateNode(type);
                    if (!add_child_name_[nid].empty())
                        child->SetName(add_child_name_[nid]);
                    node->AddChild(std::move(child));
                }
            }

            // Remove button (not for root)
            if (node->GetParent())
            {
                ImGui::SameLine();
                if (ImGui::Button("Remove"))
                {
                    if (selected_node_ == node)
                        selected_node_ = nullptr;
                    node->GetParent()->RemoveChild(node);
                    ImGui::PopID();
                    ImGui::TreePop();
                    return;
                }
            }

            ImGui::PopID();

            for (auto& child : node->GetChildren())
            {
                DrawNodesRecursive(child.get());
            }
            ImGui::TreePop();
        }
    }

    void DrawInspector()
    {
        if (ImGui::Begin("Inspector"))
        {
            if (selected_node_)
            {
                ImGui::Text("Name: %s", selected_node_->GetName().c_str());
                ImGui::Text("Type: %s", selected_node_->GetTypeName().c_str());
                ImGui::Separator();

                // Check if it's a Node3D
                if (auto* node3d = dynamic_cast<rle::Node3D*>(selected_node_))
                {
                    auto transform = node3d->GetLocalTransform();

                    float pos[3] = {transform.translation.x, transform.translation.y, transform.translation.z};
                    if (ImGui::DragFloat3("Position", pos, 0.1f))
                        node3d->SetPosition({pos[0], pos[1], pos[2]});

                    // Show rotation as euler angles
                    Vector3 euler = QuaternionToEuler(transform.rotation);
                    float rot[3] = {euler.x * RAD2DEG, euler.y * RAD2DEG, euler.z * RAD2DEG};
                    if (ImGui::DragFloat3("Rotation", rot, 1.0f))
                    {
                        Quaternion q = QuaternionFromEuler(rot[0] * DEG2RAD, rot[1] * DEG2RAD, rot[2] * DEG2RAD);
                        node3d->SetRotation(q);
                    }

                    float scl[3] = {transform.scale.x, transform.scale.y, transform.scale.z};
                    if (ImGui::DragFloat3("Scale", scl, 0.1f))
                        node3d->SetScale({scl[0], scl[1], scl[2]});
                }

                // Check if it's a Node2D
                if (auto* node2d = dynamic_cast<rle::Node2D*>(selected_node_))
                {
                    auto transform = node2d->GetLocalTransform();

                    float pos[2] = {transform.translation.x, transform.translation.y};
                    if (ImGui::DragFloat2("Position", pos, 0.1f))
                        node2d->SetPosition({pos[0], pos[1]});

                    float rot = transform.rotation * RAD2DEG;
                    if (ImGui::DragFloat("Rotation", &rot, 1.0f))
                        node2d->SetRotation(rot * DEG2RAD);

                    float scl[2] = {transform.scale.x, transform.scale.y};
                    if (ImGui::DragFloat2("Scale", scl, 0.1f))
                        node2d->SetScale({scl[0], scl[1]});
                }
            }
            else
            {
                ImGui::Text("No node selected.");
            }
        }
        ImGui::End();
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