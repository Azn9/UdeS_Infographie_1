#include "../../Public/Debug/FPSDebugRenderer.h"

#include "../../../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../../../PetitMoteur3D/Core/Public/Core/moteurWindows.h"

void FPSDebugRenderer::Draw()
{
    if (lastUpdate == 0)
    {
        lastUpdate = PM3D::CMoteurWindows::GetInstance().GetTimeSpecific();
        return;
    }
    
    const int64_t currentTime = PM3D::CMoteurWindows::GetInstance().GetTimeSpecific();
    const double msDelta = PM3D::CMoteurWindows::GetInstance().GetTimeIntervalsInSec(lastUpdate, currentTime) * 1000.0;

    const double fps = 1000.0 / msDelta;
    
    ImGui::Begin("FPS");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", msDelta, fps);
    ImGui::Text(("Last frame took " + std::to_string(PM3D::CMoteurWindows::GetInstance().GetLastFrameTime()) + "ms to draw").c_str());
    ImGui::Text(("Last present took " + std::to_string(PM3D::CMoteurWindows::GetInstance().GetPresentTime()) + "ms").c_str());
    
    ImGui::End();

    lastUpdate = currentTime;
}
