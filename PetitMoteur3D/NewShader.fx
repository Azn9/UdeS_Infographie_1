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
};

cbuffer param
{
    float4x4 matWorldViewProj; // la matrice totale 
    float4x4 matWorld; // matrice de transformation dans le monde
    float4 vCamera; // la position de la caméra
    float4 vAMat; // la valeur ambiante du matériau
    float4 vDMat; // la valeur diffuse du matériau
    float4 vSMat; // la valeur spéculaire du matériau
    float puissance; // la puissance de spécularité
    int bTex; // Booléen pour la présence de texture
    float2 remplissage;
};

#define MAX_LIGHTS 10
StructuredBuffer<Light> lights;

Texture2D textureEntree; // la texture
SamplerState SampleState; // l’état de sampling

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm :    TEXCOORD0;
	float3 vDirCam : TEXCOORD1;
	float2 coordTex : TEXCOORD2;
	float3 posWorld : TEXCOORD3;
};

VS_Sortie MainVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex: TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;

	sortie.Pos = mul(Pos, matWorldViewProj);
	sortie.Norm = mul(float4(Normale, 0.0f), matWorld).xyz;

	float3 PosWorld = mul(Pos, matWorld).xyz;
	sortie.posWorld = PosWorld;

	sortie.vDirCam = vCamera.xyz - PosWorld;

	// Coordonnées d’application de texture
	sortie.coordTex = coordTex;

	return sortie;
}

float4 MainPS(VS_Sortie vs) : SV_Target
{
	float3 totalAmbiant = float3(0, 0, 0);
	float3 totalDiffuse = float3(0, 0, 0);
	float3 totalSpecular = float3(0, 0, 0);

	float3 N = normalize(vs.Norm);
	float3 V = normalize(vs.vDirCam);

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];

		if (!li.initialized) {
			continue;
        }

		if (li.lightType == 0) // ambiant
		{
			
		}
		else if (li.lightType == 1) // Directionnal
		{
			float3 L = normalize(-li.direction.xyz);
            float3 diff = saturate(dot(N, L));
            float3 R = normalize(2 * diff * N - L);
			float3 S = pow(saturate(dot(R, V)), li.specularPower);

			totalDiffuse += li.diffuse.rgb * diff;
			totalSpecular += li.specular.rgb * S;
		}
		else if (li.lightType == 2) // Point
		{
			float3 lightDirection = normalize(li.position.xyz - vs.posWorld.xyz);
			float3 unnormalisedLightDirection = li.position.xyz - vs.posWorld.xyz;

			float diffuseFalloff1 = 1 / dot(unnormalisedLightDirection, unnormalisedLightDirection);
			float diffuseFalloff2 = 1 / (length(unnormalisedLightDirection) * length(unnormalisedLightDirection)) / dot(unnormalisedLightDirection, unnormalisedLightDirection);

			totalAmbiant += li.ambiant.xyz * diffuseFalloff2;
		
			float3 diff = saturate(dot(N, lightDirection));
			float3 R = normalize(2 * diff * N - lightDirection);
			float3 S = pow(saturate(dot(R, V)), li.specularPower);

			totalDiffuse += li.diffuse.rgb * diff * diffuseFalloff2;
			totalSpecular += li.specular.rgb * S;
		}
		else if (li.lightType == 3) // Spot
		{
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
		}
	}

	// Échantillonner la couleur du pixel à partir de la texture
	float3 couleurTexture = float3(1, 1, 1);

	if (bTex > 0)
    {
        // Échantillonner la couleur du pixel à partir de la texture
        couleurTexture = textureEntree.Sample(SampleState, vs.coordTex).rgb;
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
