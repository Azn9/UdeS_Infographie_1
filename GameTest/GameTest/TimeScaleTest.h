#pragma once
#include "../../PetitMoteur3D/Core/Imgui/imgui.h"
#include "../../PetitMoteur3D/Core/Public/Util/Time.h"
#include "../../PM3DApi/Api/Public/Debug/DebugRenderer.h"

class TimeScaleTest final : public PM3D_API::DebugRenderer
{
public:
	TimeScaleTest() : DebugRenderer() {}
	~TimeScaleTest() override = default;

	void Draw() override
	{
		ImGui::Text("TimeScale");
		ImGui::SliderFloat("SliderFloat (0 -> 1)", PM3D::Time::GetInstance().GetTimeScalePtr(), 0.0f, 2.0f, "%.1f");
	}
};