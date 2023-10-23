#include "../../Public/Debug/FPSDebugRenderer.h"

#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../../../PetitMoteur3D/Core/Public/Core/moteurWindows.h"

void PM3D_API::FPSDebugRenderer::Draw()
{
    if (lastUpdate == 0)
    {
        lastUpdate = Time::GetTimeSpecific();
        return;
    }
    
    const int64_t currentTime = Time::GetTimeSpecific();
    const double msDelta = Time::GetTimeIntervalsInMs(lastUpdate, currentTime);

    const double fps = 1000.0 / msDelta;
    
    ImGui::Begin("FPS");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", msDelta, fps);
    ImGui::Text(("Last frame took " + std::to_string(PM3D::CMoteurWindows::GetInstance().GetLastFrameTime()) + "ms to draw").c_str());
    ImGui::Text(("Last present took " + std::to_string(PM3D::CMoteurWindows::GetInstance().GetPresentTime()) + "ms").c_str());
    
    ImGui::End();

    lastUpdate = currentTime;
}
