#define EPSILON 0.000001
struct Light {
	int initialized;

	int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot

	float2 padding;
	
	float4 position;
	float4 direction;
	
	float4 ambiant;
	float4 diffuse;
	float4 specular;
	float specularPower;

	// Only for spot
	float innerAngle;
	float outerAngle;

	float padding2;

	float4x4 matWorldViewProj;

	//float4 padding;
	//float3 padding2;
	//float4 padding3;
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
SamplerState SampleState; // l’état de sampling

Texture2D shadowTexture; // La texture du shadow map
SamplerState ShadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

Texture2D normalMap;
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
	float4 PosInMap[MAX_LIGHTS] : TEXCOORD7;
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

	float4 PosInMaps[MAX_LIGHTS];

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];
		
		if (!li.initialized) continue;
		
		float4 tempValue = mul(vPos, li.matWorldViewProj);
		PosInMaps[i] = tempValue;
	}

	output.PosInMap = PosInMaps;

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
			totalAmbiant += li.ambiant;
		}
		else if (li.lightType == 1) // Directionnal
		{
			/*
			float3 L = normalize(-li.direction.xyz);
            float3 diff = saturate(dot(N, L));
            float3 R = normalize(2 * diff * N - L);
			float3 S = pow(saturate(dot(R, V)), li.specularPower);

			totalDiffuse += li.diffuse.rgb * diff;
			totalSpecular += li.specular.rgb * S;
			*/
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

            float puissance = max(EPSILON, Ns * li.specularPower);
            float specularValue = pow(NdotH, puissance);

            // Attenuation
            float D = length(li.position.xyz - input.worldPos);
            float attenuation = (1 / D) * li.specularPower;
            //float attenuation = (1 / (D * D)) * li.intensity;

            totalAmbiant += li.ambiant * attenuation;
            totalDiffuse += li.diffuse * diffuseValue * attenuation;
            totalSpecular += li.specular * specularValue * attenuation;
		}
		else if (li.lightType == 3) // Spot
		{
			/*
			float3 lightDirection = normalize(li.position.xyz - vs.posWorld.xyz);
			float3 spotDirection = normalize(li.direction.xyz);

			float spotCosine = saturate(dot(lightDirection, -spotDirection));

			if (spotCosine >= cos(li.outerAngle))
			{
				float diffuseFalloff;

				if (spotCosine >= cos(li.innerAngle))
				{
					diffuseFalloff = 1.0;
				}
				else
				{
					float t = saturate((spotCosine - cos(li.outerAngle)) / (cos(li.innerAngle) - cos(li.outerAngle)));
        			diffuseFalloff = smoothstep(0.0, 1.0, t);
				}

				totalAmbiant += li.ambiant.xyz * diffuseFalloff;

				float3 diff = saturate(dot(N, normalize(vs.posWorld.xyz - li.position.xyz)));
				float3 R = normalize(2 * diff * N - lightDirection);
				float3 S = pow(saturate(dot(R, V)), li.specularPower);

				totalDiffuse += li.diffuse.rgb * diff * diffuseFalloff;
				totalSpecular += li.specular.rgb * S;
			}
			*/
		}

		// SHADOWS


	}

	// Échantillonner la couleur du pixel à partir de la texture
	float3 couleurTexture = float3(1, 1, 1);

	if (hasAlbedoTexture > 0)
    {
        // Échantillonner la couleur du pixel à partir de la texture
        couleurTexture = textureEntree.Sample(SampleState, input.coordTex).rgb;
    }

	float3 finalColor = couleurTexture.rgb * (totalAmbiant /** vAMat.rgb*/ + totalDiffuse /** vDMat.rgb*/ + totalSpecular /** vSMat.rgb*/);

	return float4(finalColor, 1.0f);
}

technique11 NewShader
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_5_0, MainVS()));
		SetPixelShader(CompileShader(ps_5_0, MainPS()));
		SetGeometryShader(NULL);
	}
}


// ===========

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

	float profondeur[MAX_LIGHTS];

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];

		if (!li.initialized) continue;

		float4 pos = mul(Pos, li.matWorldViewProj);
		float profondeurV = pos.z / pos.w;

		profondeur[i] = profondeurV;
	}

	Out.profondeur = profondeur;

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
