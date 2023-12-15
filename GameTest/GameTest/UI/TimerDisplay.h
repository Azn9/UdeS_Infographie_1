#pragma once
#include "Api/Public/Component/Basic/Render/2D/TextRenderer.h"
#include "Api/Public/GameObject/UIObject.h"

class TimerDisplay final : public PM3D_API::UIObject
{
public:
    TimerDisplay()
        : UIObject(
              "Timer display",
              DirectX::XMFLOAT2(
                  386.f / 1920.f,
                  68.f / 1080.f
              ),
              DirectX::XMFLOAT2(
                  1600.f / 1920.f,
                  1000.f / 1080.f
              ),
              0.0f,
              true,
              true
          ), startTime(0), endTime(0), isRunning(false), textDisplayPtr(nullptr), isInTunnel(false)
    {
    }

    void Initialize() override;
    void Update() override;

    void Test()
    {
        isInTunnel = true;
    }

    void Test2()
    {
        std::cout << "aaa" << std::endl;
    }

    void Test3()
    {
        std::cout << "aaa" << std::endl;
    }

private:
    int64_t time;
    int64_t startTime;
    int64_t endTime;
    bool isRunning;
    bool isInTunnel;
    std::unique_ptr<PM3D_API::TextRenderer>::pointer textDisplayPtr;
};
