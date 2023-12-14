#pragma once
#include <codecvt>

#include "Renderer.h"
#include "Api/Public/Shader/Basic/DefaultShader.h"
#include "Api/Public/Shader/Basic/SpriteShader.h"

namespace PM3D
{
    class CTexture;
}

namespace PM3D_API
{
    enum class BillboardAlignment
    {
        Z,
        XZ,
        YZ,
        XYZ
    };

    class BillboardRenderer final : public Renderer
    {
    public:
        explicit BillboardRenderer(
            std::wstring textureName
        ) : Renderer(std::make_unique<SpriteShader>(L"shader/Sprite1.fx"))
            , textureName(std::move(textureName))
            , alignment(BillboardAlignment::Z)
            , texture(nullptr)
            , selfScale(DirectX::XMFLOAT2(1.0f, 1.0f))
        {
        }

        BillboardRenderer(
            std::wstring textureName,
            const DirectX::XMFLOAT2& selfScale
        ) : Renderer(std::make_unique<SpriteShader>(L"shader/Sprite1.fx"))
            , textureName(std::move(textureName))
            , alignment(BillboardAlignment::Z)
            , texture(nullptr)
            , selfScale(selfScale)
        {
        }

        BillboardRenderer(
            std::wstring textureName,
            const BillboardAlignment& alignment
        ) : Renderer(std::make_unique<SpriteShader>(L"shader/Sprite1.fx"))
            , textureName(std::move(textureName))
            , alignment(alignment)
            , texture(nullptr)
            , selfScale(DirectX::XMFLOAT2(1.0f, 1.0f))
        {
        }

        BillboardRenderer(
            std::wstring textureName,
            const BillboardAlignment& alignment,
            const DirectX::XMFLOAT2& selfScale
        ) : Renderer(std::make_unique<SpriteShader>(L"shader/Sprite1.fx"))
            , textureName(std::move(textureName))
            , alignment(alignment)
            , texture(nullptr)
            , selfScale(selfScale)
        {
        }

        BillboardRenderer(
            std::unique_ptr<Shader>&& shader,
            std::wstring textureName
        ) : Renderer(std::move(shader))
            , textureName(std::move(textureName))
            , alignment(BillboardAlignment::Z)
            , texture(nullptr)
            , selfScale(DirectX::XMFLOAT2(1.0f, 1.0f))
        {
        }

        BillboardRenderer(
            std::unique_ptr<Shader>&& shader,
            std::wstring textureName,
            const DirectX::XMFLOAT2& selfScale
        ) : Renderer(std::move(shader))
            , textureName(std::move(textureName))
            , alignment(BillboardAlignment::Z)
            , texture(nullptr)
            , selfScale(selfScale)
        {
        }

        BillboardRenderer(
            std::unique_ptr<Shader>&& shader,
            std::wstring textureName,
            const BillboardAlignment& alignment
        ) : Renderer(std::move(shader))
            , textureName(std::move(textureName))
            , alignment(alignment)
            , texture(nullptr)
            , selfScale(DirectX::XMFLOAT2(1.0f, 1.0f))
        {
        }

        BillboardRenderer(
            std::unique_ptr<Shader>&& shader,
            std::wstring textureName,
            const BillboardAlignment& alignment,
            const DirectX::XMFLOAT2& selfScale
        ) : Renderer(std::move(shader))
            , textureName(std::move(textureName))
            , alignment(alignment)
            , texture(nullptr)
            , selfScale(selfScale)
        {
        }

        ~BillboardRenderer() override;

        void Initialize() override;
        void DrawSelf() const override;

        void DrawDebugInfo() const override
        {
            // wstring converter
#pragma warning(push)
#pragma warning(disable:4996)
            static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
#pragma warning(pop)
            static auto alignments = {"Z", "XZ", "YZ", "XYZ"};

            ImGui::Text("TextureName");
            ImGui::SameLine(200.0f);
#pragma warning(push)
#pragma warning(disable:4996)
            ImGui::Text(converter.to_bytes(textureName).c_str());
#pragma warning(pop)

            ImGui::Text("Alignment");
            ImGui::SameLine(200.0f);
            ImGui::Text(*(alignments.begin() + static_cast<int>(alignment)));
        }

    private:
        std::wstring textureName;
        BillboardAlignment alignment;
        PM3D::CTexture* texture;
        DirectX::XMFLOAT2 selfScale;
        float textureSizeX;
        float textureSizeY;
    };
}
