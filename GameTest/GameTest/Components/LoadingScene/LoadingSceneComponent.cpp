#include "LoadingSceneComponent.h"

#include "Api/Public/GameHost.h"
#include "Api/Public/Component/Basic/Render/2D/SpriteRenderer.h"
#include "Api/Public/Input/Input.h"
#include "GameTest/Scenes/MainScene.h"

void LoadingSceneComponent::Initialize()
{
	// Load scene in the background
	loadingThread = std::thread([this]()
	{
		const auto mainScenePtr = MainScene::GetInstancePtr();
		mainScenePtr->Initialize();
        
		isLoaded = true;
	});
	loadingThread.detach();

	Input::AddKeyPressedListener([this](const KeyCode& key)
	{
		if (key == KeyCode::ENTER)
		{
			wantToChangeScene = true;
		}
	});

	constexpr auto loadingTextSize = DirectX::XMFLOAT2(988.f, 123.f);
	constexpr auto expectedScreenSize = DirectX::XMFLOAT2(1920.f, 1080.f);
	constexpr auto loadingTextPosition = DirectX::XMFLOAT2(
		(expectedScreenSize.x - loadingTextSize.x) / 2.f,
		(expectedScreenSize.y - loadingTextSize.y) / 2.f
	);
	constexpr auto positionPercent = DirectX::XMFLOAT2(
		loadingTextPosition.x / expectedScreenSize.x,
		loadingTextPosition.y / expectedScreenSize.y
	);
	constexpr auto scalePercent = DirectX::XMFLOAT2(
		loadingTextSize.x / expectedScreenSize.x,
		loadingTextSize.y / expectedScreenSize.y
	);

	auto loadingTextComponent = std::make_unique<PM3D_API::UIObject>(
		"LoadingText",
		scalePercent,
		positionPercent,
		0.f,
		true,
		true
	);
	const auto loadingTextComponentPtr = loadingTextComponent.get();
	parentObject->GetScene()->GetUICanvas()->AddChild(std::move(loadingTextComponent));
	loadingTextComponentPtr->Initialize();

	auto spriteRenderer = std::make_unique<PM3D_API::SpriteRenderer>(L"loading_text.dds");
	const auto spriteRendererPtr = spriteRenderer.get();
	loadingTextComponentPtr->AddComponent(std::move(spriteRenderer));
	spriteRendererPtr->Initialize();
}

void LoadingSceneComponent::Update()
{
	if (isLoaded && wantToChangeScene)
		PM3D_API::GameHost::GetInstance()->SetScene(MainScene::GetInstancePtr());
}
