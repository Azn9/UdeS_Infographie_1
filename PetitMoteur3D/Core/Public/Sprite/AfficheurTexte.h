#pragma once

#include "Core/Public/Core/dispositifD3D11.h"
#include <Gdiplus.h>
#include <memory>
#include <string>
#pragma comment(lib, "gdiplus.lib")

namespace PM3D
{
    class CAfficheurTexte
    {
    public :
        CAfficheurTexte(PM3D::CDispositifD3D11* pDispositif, int largeur, int hauteur, Gdiplus::Font* pPolice);
        ~CAfficheurTexte();

        void Ecrire(const std::wstring& s);

        ID3D11ShaderResourceView* GetTextureView() const { return pTextureView; }

        static void Init();
        static void Close();

    private:
        UINT TexWidth;
        UINT TexHeight;
        ID3D11Texture2D* pTexture;
        IDXGISurface1* pSurface;
        ID3D11ShaderResourceView* pTextureView;
        PM3D::CDispositifD3D11* pDispo;
        Gdiplus::Font* pFont;
        std::unique_ptr<Gdiplus::Bitmap> pCharBitmap;
        std::unique_ptr<Gdiplus::Graphics> pCharGraphics;
        std::unique_ptr<Gdiplus::SolidBrush> pBlackBrush;

        // Variables statiques pour GDI+
        static ULONG_PTR token;
    };
}
