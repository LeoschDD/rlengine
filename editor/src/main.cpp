#include <rlengine.hpp>
#include "Sandbox.hpp"

class EditorApplication : public rle::Application
{
private:
    RenderTexture2D viewport_{};
    bool initialized_{false};
    bool layout_set_{false};
    rle::Node* selected_node_{nullptr};

    ImVec2 viewport_pos_{};
    ImVec2 viewport_size_{};
    ImVec2 inspector_pos_{};
    ImVec2 inspector_size_{};

    bool show_save_dialog_{false};
    bool show_load_dialog_{false};
    char scene_name_buf_[128] = "main";

    std::unordered_map<uint32_t, std::string> add_child_type_;
    std::unordered_map<uint32_t, std::string> add_child_name_;

protected:
    void Render() override
    {
        if (!initialized_)
        {
            viewport_ = LoadRenderTexture(1280, 720);
            SetTextureFilter(viewport_.texture, TEXTURE_FILTER_BILINEAR);

            ImGuiStyle& style = ImGui::GetStyle();
            ImGui::StyleColorsDark();
            style.WindowRounding = 0.0f;
            style.FrameRounding = 4.0f;
            style.FramePadding = ImVec2(8, 4);
            style.WindowPadding = ImVec2(8, 8);
            style.ItemSpacing = ImVec2(8, 6);

            initialized_ = true;
        }
        ClearBackground(DARKGRAY);

        rlImGuiBegin();
        BeginTextureMode(viewport_);
        rle::Application::Render();
        EndTextureMode();

        DrawMenuBar();
        SetupLayout();
        DrawNodeTree();
        DrawViewport();
        DrawInspector();
        DrawFileDialog();
        rlImGuiEnd();
    };

    void SetupLayout()
    {
        if (layout_set_) return;
        layout_set_ = true;

        float screen_w = (float)GetScreenWidth();
        float screen_h = (float)GetScreenHeight();
        float menu_h = ImGui::GetFrameHeight();
        float panel_w = screen_w * 0.2f;
        float center_w = screen_w - panel_w * 2.0f;

        viewport_pos_ = ImVec2(panel_w, menu_h);
        viewport_size_ = ImVec2(center_w, screen_h - menu_h);

        inspector_pos_ = ImVec2(panel_w + center_w, menu_h);
        inspector_size_ = ImVec2(panel_w, screen_h - menu_h);
    }

    void DrawMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                {
                    if (auto* scene = GetSceneManager().GetScene())
                    {
                        strncpy(scene_name_buf_, scene->GetName().c_str(), sizeof(scene_name_buf_) - 1);
                        show_save_dialog_ = true;
                    }
                }
                if (ImGui::MenuItem("Load Scene", "Ctrl+L"))
                {
                    show_load_dialog_ = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void DrawFileDialog()
    {
        if (show_save_dialog_)
        {
            ImGui::OpenPopup("Save Scene");
            show_save_dialog_ = false;
        }
        if (show_load_dialog_)
        {
            ImGui::OpenPopup("Load Scene");
            show_load_dialog_ = false;
        }

        ImVec2 center = ImVec2((float)GetScreenWidth() * 0.5f, (float)GetScreenHeight() * 0.5f);
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(400, 0));
        if (ImGui::BeginPopupModal("Save Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Scene name:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##save_name", scene_name_buf_, sizeof(scene_name_buf_));
            ImGui::Spacing();

            if (ImGui::Button("Save", ImVec2(120, 0)))
            {
                std::string path = std::string(SCENE_DIR) + "/" + scene_name_buf_ + ".rlscene";
                GetSceneManager().SaveScene(path);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(400, 0));
        if (ImGui::BeginPopupModal("Load Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Scene name:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##load_name", scene_name_buf_, sizeof(scene_name_buf_));
            ImGui::Spacing();

            if (ImGui::Button("Load", ImVec2(120, 0)))
            {
                std::string path = std::string(SCENE_DIR) + "/" + scene_name_buf_ + ".rlscene";
                selected_node_ = nullptr;
                add_child_type_.clear();
                add_child_name_.clear();
                GetSceneManager().LoadScene(path);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void DrawViewport()
    {
        ImGui::SetNextWindowPos(viewport_pos_, ImGuiCond_Always);
        ImGui::SetNextWindowSize(viewport_size_, ImGuiCond_Always);
        if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            ImVec2 avail = ImGui::GetContentRegionAvail();
            int w = (int)avail.x, h = (int)avail.y;
            if (w > 0 && h > 0 && (w != viewport_.texture.width || h != viewport_.texture.height))
            {
                UnloadRenderTexture(viewport_);
                viewport_ = LoadRenderTexture(w, h);
                SetTextureFilter(viewport_.texture, TEXTURE_FILTER_BILINEAR);
            }
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
        float screen_w = (float)GetScreenWidth();
        float screen_h = (float)GetScreenHeight();
        float menu_h = ImGui::GetFrameHeight();

        ImGui::SetNextWindowPos(ImVec2(0, menu_h), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(screen_w * 0.2f, screen_h - menu_h), ImGuiCond_Always);
        if (ImGui::Begin("Nodes", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
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

            if (add_child_type_.find(nid) == add_child_type_.end())
                add_child_type_[nid] = "Node";
            if (add_child_name_.find(nid) == add_child_name_.end())
                add_child_name_[nid] = "";

            ImGui::PushID(nid);

            ImGui::SetNextItemWidth(100);
            char type_buf[128];
            strncpy(type_buf, add_child_type_[nid].c_str(), sizeof(type_buf) - 1);
            type_buf[sizeof(type_buf) - 1] = '\0';
            if (ImGui::InputText("Type", type_buf, sizeof(type_buf)))
                add_child_type_[nid] = type_buf;

            ImGui::SameLine();

            ImGui::SetNextItemWidth(100);
            char name_buf[128];
            strncpy(name_buf, add_child_name_[nid].c_str(), sizeof(name_buf) - 1);
            name_buf[sizeof(name_buf) - 1] = '\0';
            if (ImGui::InputText("Name", name_buf, sizeof(name_buf)))
                add_child_name_[nid] = name_buf;

            ImGui::SameLine();

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
        ImGui::SetNextWindowPos(inspector_pos_, ImGuiCond_Always);
        ImGui::SetNextWindowSize(inspector_size_, ImGuiCond_Always);
        if (ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
        {
            if (selected_node_)
            {
                ImGui::Text("Name: %s", selected_node_->GetName().c_str());
                ImGui::Text("Type: %s", selected_node_->GetTypeName().c_str());
                ImGui::Separator();

                if (auto* node3d = dynamic_cast<rle::Node3D*>(selected_node_))
                {
                    auto transform = node3d->GetLocalTransform();

                    float pos[3] = {transform.translation.x, transform.translation.y, transform.translation.z};
                    if (ImGui::DragFloat3("Position", pos, 0.1f))
                        node3d->SetPosition({pos[0], pos[1], pos[2]});

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

                    if (auto* camera_3d = dynamic_cast<rle::NodeCamera3D*>(selected_node_))
                    {
                        if (ImGui::Button("Activate"))
                        {
                            rle::Application::Get().GetSceneManager().SetActiveCamera(camera_3d);
                        }
                    }
                }

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

                    if (auto* camera_2d = dynamic_cast<rle::NodeCamera2D*>(selected_node_))
                    {
                        if (ImGui::Button("Activate"))
                        {
                            rle::Application::Get().GetSceneManager().SetActiveCamera(camera_2d);
                        }
                    }
                }
            }
            else
            {
                ImGui::TextDisabled("No node selected.");
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

int main()
{
    auto editor_app = std::make_unique<EditorApplication>();
    editor_app->Run();
}
