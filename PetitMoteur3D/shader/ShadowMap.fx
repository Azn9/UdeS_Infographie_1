struct Light {
	bool initialized;

	int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot
	
	float4 position;
	float4 direction;
	
	float4 ambiant;
	float4 diffuse;
	float4 specular;
	float specularPower;

	// Only for spot
	float innerAngle;
	float outerAngle;

	float4x4 matWorldViewProj;

	float4 padding;
	float3 padding2;
};

#define MAX_LIGHTS 10
StructuredBuffer<Light> lights;

struct ShadowMapVS_SORTIE
{
	float4 Pos 					 : SV_POSITION;
	float profondeur[MAX_LIGHTS] : TEXCOORD1;
};

ShadowMapVS_SORTIE MainVS_SM(
	float4 Pos       : POSITION, 
    float3 Normale   : NORMAL, 
    float3 vBiNormal : BINORMAL,
    float3 vTangent  : TANGENT,
    float2 coordTex  : TEXCOORD
) {
	ShadowMapVS_SORTIE Out;
	
	// Calcul des coordonnées
	Out.Pos = Pos;

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];

		if (!li.initialized) continue;

		float4 pos = mul(Pos, li.matWorldViewProj);
		float profondeur = pos.z / pos.w;

		Out.profondeur[i] = profondeur;
	}

	return Out;
}

RasterizerState rsCullFront
{
	CullMode = Front;
};

technique11 ShadowMap
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MainVS_SM()));
		SetRasterizerState(rsCullFront);

		SetPixelShader(NULL);
		SetGeometryShader(NULL);
	}
}
