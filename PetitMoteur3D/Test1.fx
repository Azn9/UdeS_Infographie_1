struct Light {
	bool initialized;
	
	int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot
	
	float4 position;
	float4 direction;
	
	float4 ambiantColor;
	float4 diffuseColor;
	float4 specularColor;
	float intensity;

	// Only for spot
	float innerAngle;
	float outerAngle;

    float4x4 matWorldViewProj;
	
	float4 padding;
	float3 padding2;
};

cbuffer param
{
    float4x4 matWorldViewProj; // la matrice totale 
    float4x4 matWorld; // matrice de transformation dans le monde
    float4 vCamera; // la position de la caméra
    float4 dCamera; // La direction de la caméra
    float4 vAMat; // la valeur ambiante du matériau
    float4 vDMat; // la valeur diffuse du matériau
    float4 vSMat; // la valeur spéculaire du matériau
    float Ns; // la puissance de spécularité
    
    int hasAlbedoTexture; // Booléen pour la présence de texture
    int hasNormalmapTexture;
};

#define MAX_LIGHTS 10
StructuredBuffer<Light> lights;

Texture2D textureEntree; // la texture
Texture2D normalMap;
SamplerState SampleState; // l’état de sampling
SamplerState SampleStateNormalMap;

struct VS_Sortie
{
	float4 position  : SV_Position;
	float3 normal    : TEXCOORD0;
    float3 binormal  : TEXCOORD1;
    float3 tangent   : TEXCOORD2;
	float3 cameraVec : TEXCOORD3;
	float2 coordTex  : TEXCOORD4;
	float3 worldPos  : TEXCOORD5;
	float3 cameraDir : TEXCOORD6;
};

VS_Sortie MainVS(
    float4 vPos      : POSITION, 
    float3 vNormal   : NORMAL, 
    float3 vBiNormal : BINORMAL,
    float3 vTangent  : TANGENT,
    float2 coordTex  : TEXCOORD
) {
    VS_Sortie output;

    output.normal = mul(float4(vNormal, 0.0f), matWorld);
    output.binormal = mul(float4(vBiNormal, 0.0f), matWorld);
    output.tangent = mul(float4(vTangent, 0.0f), matWorld);

    output.position = mul(vPos, matWorldViewProj);
    output.worldPos = mul(vPos, matWorld);

    output.coordTex = coordTex;

    output.cameraVec = vCamera.xyz - output.worldPos;
    output.cameraDir = dCamera.xyz;

    return output;
}

float4 MainPS(VS_Sortie input) : SV_Target
{
    float3 normal = input.normal;
    float3 binormal = input.binormal;
    float3 tangent = input.tangent;

    if (hasNormalmapTexture) {
        // TODO
    }

    float3 totalAmbiant = float3(0, 0, 0);
	float3 totalDiffuse = float3(0, 0, 0);
	float3 totalSpecular = float3(0, 0, 0);

    float3 N = normalize(normal);
	float3 V = normalize(input.cameraDir);

    for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];

		if (!li.initialized) {
			continue;
        }

        if (li.lightType == 0) // ambiant
		{
            totalAmbiant += li.ambiantColor;
            continue;
		}
		else if (li.lightType == 1) // Directionnal
		{
            float3 L = normalize(li.direction.xyz);
            
            // Diffuse
            float diffuseValue = saturate(dot(N, L));

            // Specular
            float3 H = normalize(L + V);
            float NdotH = saturate(dot(N, H));

            float puissance = Ns * li.intensity;
            float specularValue = pow(NdotH, puissance);

            totalAmbiant += li.ambiantColor;
            totalDiffuse += li.diffuseColor * diffuseValue;
            totalSpecular += li.specularColor * specularValue;
        }
		else if (li.lightType == 2) // Point
		{
            float3 L = normalize(li.position.xyz - input.worldPos);

            // Diffuse
            float diffuseValue = saturate(dot(N, L));

            // Specular
            //float3 R = -reflect(L, N);
            //float3 RdotV = saturate(dot(R, V));
            //float S = pow(RdotV, puissance);
            float3 H = normalize(L + V);
            float NdotH = saturate(dot(N, H));

            float puissance = Ns * li.intensity;
            float specularValue = pow(NdotH, puissance);

            // Attenuation
            float D = length(li.position.xyz - input.worldPos);
            float attenuation = (1 / D) * li.intensity;
            //float attenuation = (1 / (D * D)) * li.intensity;

            totalAmbiant += li.ambiantColor * attenuation;
            totalDiffuse += li.diffuseColor * diffuseValue * attenuation;
            totalSpecular += li.specularColor * specularValue * attenuation;
        }
		else if (li.lightType == 3) // Spot
		{

        }
    }

    float3 couleurTexture = float3(1, 1, 1);

    if (hasAlbedoTexture)
    {
        couleurTexture = textureEntree.Sample(SampleState, input.coordTex).rgb;
    }

    float3 finalColor = couleurTexture.rgb * (totalAmbiant /** vAMat.rgb*/ + totalDiffuse /** vDMat.rgb*/ + totalSpecular /** vSMat.rgb*/);

    return float4(finalColor, 1.0f);
}

technique11 Test1
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MainVS()));
		SetPixelShader(CompileShader(ps_5_0, MainPS()));
		SetGeometryShader(NULL);
	}
}

technique11 ShadowMap
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MainVS()));
		SetPixelShader(CompileShader(ps_5_0, MainPS()));
		SetGeometryShader(NULL);
	}
}