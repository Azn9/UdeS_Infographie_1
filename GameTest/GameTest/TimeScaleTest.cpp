#include "TimeScaleTest.h"

#include "Core/Public/Core/MoteurWindows.h"
#include "Core/Public/Util/Time.h"

void TimeScaleTest::StepOneFrame()
{
	std::cout << "StepOneFrame" << std::endl;
	PM3D::CMoteurWindows::GetInstance().StepOneFrame();
}

void TimeScaleTest::Draw()
{
	ImGui::Text("TimeScale");
	ImGui::SliderFloat("SliderFloat (0 -> 1)", PM3D::Time::GetInstance().GetTimeScalePtr(), 0.0f, 2.0f, "%.1f");

	ImGui::Button("Step 1 frame");
	if (ImGui::IsItemClicked())
	{
		StepOneFrame();
	}
}
