cbuffer param
{
    float4x4 matWVP; // la matrice de travail
    float3 color;
    float alpha;
    float percent;
}

struct VS_Sortie
{
    float4 Pos : SV_Position;
    float2 coordTex : TEXCOORD0;
};

VS_Sortie Sprite1VS(float4 Pos : POSITION, float2 coordTex : TEXCOORD)
{
    VS_Sortie sortie = (VS_Sortie)0;
    sortie.Pos = mul(Pos, matWVP);
    
    // Coordonn�es d�application de texture
    sortie.coordTex = coordTex;
    
    return sortie;
}

Texture2D textureEntree; // la texture
SamplerState SampleState; // l��tat de sampling

float4 Sprite1PS(VS_Sortie vs) : SV_Target
{
    float4 couleurTexture;

    if (vs.coordTex.x > percent) {
        return float4(0, 0, 0, 0);
    }
    
    couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);
    couleurTexture.r *= color.r;
    couleurTexture.g *= color.g;
    couleurTexture.b *= color.b;
    
    return float4(couleurTexture.rgb, couleurTexture.a * alpha);
}

technique11 AfficheSprite
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_5_0, Sprite1VS()));
        SetPixelShader(CompileShader(ps_5_0, Sprite1PS()));
        SetGeometryShader(NULL);
    }
}
