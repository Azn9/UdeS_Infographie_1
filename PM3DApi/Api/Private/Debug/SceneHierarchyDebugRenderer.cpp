#include "../../Public/Debug/SceneHierarchyDebugRenderer.h"

#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../../../PetitMoteur3D/Core/Public/Core/MoteurWindows.h"
#include "Api/Public/GameHost.h"

void PM3D_API::SceneHierarchyDebugRenderer::DisplayChild(const PM3D_API::GameObject* gameObject)
{
    if (!gameObject) return;

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    if (ImGui::TreeNodeEx((gameObject->GetName() + " (" + typeid(*gameObject).name() + ")").c_str(),
                          ImGuiTreeNodeFlags_SpanFullWidth))
    {
        ImGui::TableNextColumn();
        ImGui::Button("...");
        if (ImGui::IsItemClicked())
        {
            if (selectedObject == gameObject)
                selectedObject = nullptr;
            else
                selectedObject = gameObject;
        }

        for (const auto& child : gameObject->GetChildren())
        {
            if (child)
            {
                DisplayChild(child.get());
            }
        }

        ImGui::TreePop();
        ImGui::Spacing();
    }
    else
    {
        ImGui::TableNextColumn();
        ImGui::Button("...");
        if (ImGui::IsItemClicked())
        {
            if (selectedObject == gameObject)
                selectedObject = nullptr;
            else
                selectedObject = gameObject;
        }
    }
}

void PM3D_API::SceneHierarchyDebugRenderer::Draw()
{
    const auto scene = PM3D_API::GameHost::GetInstance()->GetScene();

    if (scene)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
        ImGui::Begin("Scene Hierarchy");

        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV
            | ImGuiTableFlags_BordersOuterH
            | ImGuiTableFlags_RowBg
            | ImGuiTableFlags_NoBordersInBody;
        if (ImGui::BeginTable("scene_hierarchy", 2, flags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("  ", ImGuiTableColumnFlags_WidthFixed, 30.0f);
            ImGui::TableHeadersRow();

            DisplayChild(scene);

            ImGui::EndTable();
        }

        if (selectedObject)
        {
            ImGui::Begin("Object Inspector");
            ImGui::BeginChild("Object Inspector", ImVec2(500, 500), false, ImGuiWindowFlags_AlwaysAutoResize);

            ImGui::SeparatorText("Details");
            ImGui::Text("Name");
            ImGui::SameLine(100);
            ImGui::Text(selectedObject->GetName().c_str());

            ImGui::Text("Type");
            ImGui::SameLine(100);
            ImGui::Text(typeid(*selectedObject).name());

            // == Draw ==
            ImGui::SeparatorText("Draw");

            ImGui::Text("Self");
            ImGui::SameLine(100);
            ImGui::Text((std::to_string(Time::GetTimeIntervalsInMs(selectedObject->GetBeginDrawSelf(), selectedObject->GetEndDrawSelf())) + " ms").c_str());

            ImGui::Text("Components");
            ImGui::SameLine(100);

            double totalComponentsDrawTime = 0;
            for (const auto& component : selectedObject->GetComponents())
            {
                if (component)
                {
                    totalComponentsDrawTime += Time::GetTimeIntervalsInMs(component->GetBeginDrawSelf(), component->GetEndDrawSelf());
                }
            }
            ImGui::Text((std::to_string(totalComponentsDrawTime) + " ms").c_str());

            // == Transform ==
            ImGui::SeparatorText("Transform");

            ImGui::Text("Position");
            ImGui::SameLine(100);
            ImGui::Text(("x=" + std::to_string(selectedObject->GetWorldPosition().x)).c_str());
            ImGui::SameLine(200);
            ImGui::Text(("y=" + std::to_string(selectedObject->GetWorldPosition().y)).c_str());
            ImGui::SameLine(300);
            ImGui::Text(("z=" + std::to_string(selectedObject->GetWorldPosition().z)).c_str());

            ImGui::Text("Rotation");
            ImGui::SameLine(100);
            ImGui::Text(("x=" + std::to_string(selectedObject->GetWorldRotationEuler().x)).c_str());
            ImGui::SameLine(200);
            ImGui::Text(("y=" + std::to_string(selectedObject->GetWorldRotationEuler().y)).c_str());
            ImGui::SameLine(300);
            ImGui::Text(("z=" + std::to_string(selectedObject->GetWorldRotationEuler().z)).c_str());

            ImGui::Text("Scale");
            ImGui::SameLine(100);
            ImGui::Text(("x=" + std::to_string(selectedObject->GetWorldScale().x)).c_str());
            ImGui::SameLine(200);
            ImGui::Text(("y=" + std::to_string(selectedObject->GetWorldScale().y)).c_str());
            ImGui::SameLine(300);
            ImGui::Text(("z=" + std::to_string(selectedObject->GetWorldScale().z)).c_str());


            // == Local Transform ==
            ImGui::SeparatorText("Local Transform");

            ImGui::Text("Position");
            ImGui::SameLine(100);
            ImGui::Text(("x=" + std::to_string(selectedObject->GetLocalPosition().x)).c_str());
            ImGui::SameLine(200);
            ImGui::Text(("y=" + std::to_string(selectedObject->GetLocalPosition().y)).c_str());
            ImGui::SameLine(300);
            ImGui::Text(("z=" + std::to_string(selectedObject->GetLocalPosition().z)).c_str());

            ImGui::Text("Rotation");
            ImGui::SameLine(100);
            ImGui::Text(("x=" + std::to_string(selectedObject->GetLocalRotationEuler().x)).c_str());
            ImGui::SameLine(200);
            ImGui::Text(("y=" + std::to_string(selectedObject->GetLocalRotationEuler().y)).c_str());
            ImGui::SameLine(300);
            ImGui::Text(("z=" + std::to_string(selectedObject->GetLocalRotationEuler().z)).c_str());

            ImGui::Text("Scale");
            ImGui::SameLine(100);
            ImGui::Text(("x=" + std::to_string(selectedObject->GetLocalScale().x)).c_str());
            ImGui::SameLine(200);
            ImGui::Text(("y=" + std::to_string(selectedObject->GetLocalScale().y)).c_str());
            ImGui::SameLine(300);
            ImGui::Text(("z=" + std::to_string(selectedObject->GetLocalScale().z)).c_str());

            // == Own Data ==
            ImGui::SeparatorText("Data");
            selectedObject->DrawDebugInfo();

            // == Components ==
            ImGui::SeparatorText("Components");

            for (const auto& component : selectedObject->GetComponents())
            {
                if (component)
                {
                    if (ImGui::CollapsingHeader(typeid(*component.get()).name(), ImGuiTreeNodeFlags_SpanFullWidth))
                    {
                        component->DrawDebugInfo();
                    }
                }
            }

            ImGui::EndChild();
            ImGui::End();
        }

        ImGui::End();
    }
}
