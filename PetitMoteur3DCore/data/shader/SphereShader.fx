float wglnoise_mod289(float x)
{
    return x - floor(x / 289) * 289;
}

float2 wglnoise_mod289(float2 x)
{
    return x - floor(x / 289) * 289;
}

float3 wglnoise_mod289(float3 x)
{
    return x - floor(x / 289) * 289;
}

float4 wglnoise_mod289(float4 x)
{
    return x - floor(x / 289) * 289;
}

float3 wglnoise_permute(float3 x)
{
    return wglnoise_mod289((x * 34 + 1) * x);
}

float4 wglnoise_permute(float4 x)
{
    return wglnoise_mod289((x * 34 + 1) * x);
}

float4 SimplexNoiseGrad(float3 v)
{
    // First corner
    float3 i  = floor(v + dot(v, 1.0 / 3));
    float3 x0 = v   - i + dot(i, 1.0 / 6);

    // Other corners
    float3 g = x0.yzx <= x0.xyz;
    float3 l = 1 - g;
    float3 i1 = min(g.xyz, l.zxy);
    float3 i2 = max(g.xyz, l.zxy);

    float3 x1 = x0 - i1 + 1.0 / 6;
    float3 x2 = x0 - i2 + 1.0 / 3;
    float3 x3 = x0 - 0.5;

    // Permutations
    i = wglnoise_mod289(i); // Avoid truncation effects in permutation
    float4 p = wglnoise_permute(    i.z + float4(0, i1.z, i2.z, 1));
           p = wglnoise_permute(p + i.y + float4(0, i1.y, i2.y, 1));
           p = wglnoise_permute(p + i.x + float4(0, i1.x, i2.x, 1));

    // Gradients: 7x7 points over a square, mapped onto an octahedron.
    // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
    float4 gx = lerp(-1, 1, frac(floor(p / 7) / 7));
    float4 gy = lerp(-1, 1, frac(floor(p % 7) / 7));
    float4 gz = 1 - abs(gx) - abs(gy);

    bool4 zn = gz < -0.01;
    gx += zn * (gx < -0.01 ? 1 : -1);
    gy += zn * (gy < -0.01 ? 1 : -1);

    float3 g0 = normalize(float3(gx.x, gy.x, gz.x));
    float3 g1 = normalize(float3(gx.y, gy.y, gz.y));
    float3 g2 = normalize(float3(gx.z, gy.z, gz.z));
    float3 g3 = normalize(float3(gx.w, gy.w, gz.w));

    // Compute noise and gradient at P
    float4 m  = float4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3));
    float4 px = float4(dot(g0, x0), dot(g1, x1), dot(g2, x2), dot(g3, x3));

    m = max(0.5 - m, 0);
    float4 m3 = m * m * m;
    float4 m4 = m * m3;

    float4 temp = -8 * m3 * px;
    float3 grad = m4.x * g0 + temp.x * x0 +
                  m4.y * g1 + temp.y * x1 +
                  m4.z * g2 + temp.z * x2 +
                  m4.w * g3 + temp.w * x3;

    return 107 * float4(grad, dot(m4, px));
}

float SimplexNoise(float3 v)
{
    return SimplexNoiseGrad(v).w;
}




#define EPSILON 0.000001
struct Light {
	float4x4 matWorldViewProj;
	
	float4 position;
	float4 direction;
	
	float4 ambiant;
	float4 diffuse;
	float4 specular;

	float specularPower;

	// Only for spot
	float innerAngle;
	float outerAngle;

	int initialized;
	int lightType; // 0 = ambient, 1 = directional, 2 = point, 3 = spot

	float3 padding;
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
    float4 spherePos;
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
	//Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
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
    float noiseValue : TEXCOORD7;
	float4 PosInMap[MAX_LIGHTS] : TEXCOORD8;
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

    //output.position = mul(vPos, matWorldViewProj);
    output.worldPos = mul(vPos, matWorld);

    float3 vec = normalize(output.worldPos.xyz - spherePos.xyz);
    float noiseV = SimplexNoise(vPos.xyz * 5.0f) * 0.07f;
    vec *= noiseV;

    vPos.x += vec.x;
    vPos.y += vec.y;
    vPos.z += vec.z;

    output.noiseValue = noiseV;

    output.position = mul(vPos, matWorldViewProj);

    output.coordTex = coordTex;

    output.cameraVec = vCamera.xyz - output.worldPos;
    output.cameraDir = dCamera.xyz;

	float4 PosInMaps[MAX_LIGHTS];

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];
		
		if (!li.initialized) {
			PosInMaps[i] = float4(0, 0, 0, 0);
			continue;
		}
		
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
        //normal = (normal.z * normal) + (normal.x * binormal) + (normal.y * -tangent);
		//normal = normalize(normal);
    }

    float3 totalAmbiant = float3(0, 0, 0);
	float3 totalDiffuse = float3(0, 0, 0);
	float3 totalSpecular = float3(0, 0, 0);

    float3 N = normalize(normal);
	float3 V = normalize(input.cameraVec);

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];

		if (!li.initialized) {
			continue;
        }

		float3 ambiantValueF = float3(0, 0, 0);
		float3 diffuseValueF = float3(0, 0, 0);
		float3 specularValueF = float3(0, 0, 0);

		if (li.lightType == 0) // ambiant
		{
			ambiantValueF = li.ambiant;
		}
		else if (li.lightType == 1) // Directionnal
		{
			// Cheat pour ne pas éclairer dans le tunnel
			if (input.worldPos.z < -2184 && abs(length(float3(62.5323f, -1158.03f, 0.f) - float3(input.worldPos.x, input.worldPos.y, 0))) < 104.f) continue;

			float3 L = normalize(-li.direction.xyz);
			float3 diff = saturate(dot(N, L));
			//float3 R = normalize(2 * diff * N - L);
			//float3 S = pow(saturate(dot(R, V)), li.specularPower);

			float3 H = normalize(L + V);
			float NdotH = saturate(dot(N, H));
			float puissance = max(EPSILON, Ns * li.specularPower);
			float specularValue = pow(NdotH, puissance);

			ambiantValueF = li.ambiant;
			diffuseValueF = li.diffuse.rgb * diff;
			specularValueF = li.specular.rgb * specularValue;
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

            ambiantValueF = li.ambiant * attenuation;
            diffuseValueF = li.diffuse * diffuseValue * attenuation;
            specularValueF = li.specular * specularValue * attenuation;
		}
		else if (li.lightType == 3) // Spot
		{
			float3 lightDirection = normalize(li.position.xyz - input.position.xyz);
			float3 spotDirection = normalize(-li.direction.xyz);

			float spotCosine = saturate(dot(lightDirection, -spotDirection));

			if (spotCosine <= cos(li.outerAngle))
			{
				float diffuseFalloff;

				if (spotCosine <= cos(li.innerAngle))
				{
					diffuseFalloff = 1.0;
				}
				else
				{
					float t = saturate((spotCosine - cos(li.outerAngle)) / (cos(li.innerAngle) - cos(li.outerAngle)));
        			diffuseFalloff = smoothstep(0.0, 1.0, t);
				}

				float3 L = normalize(-li.direction.xyz);
				float3 diff = saturate(dot(N, L));
				//float3 R = normalize(2 * diff * N - L);
				//float3 S = pow(saturate(dot(R, V)), li.specularPower);

				float3 H = normalize(L + V);
				float NdotH = saturate(dot(N, H));
				float puissance = max(EPSILON, Ns * li.specularPower);
				float specularValue = pow(NdotH, puissance);

				ambiantValueF = li.ambiant.xyz * diffuseFalloff;
				diffuseValueF = li.diffuse.rgb * diff * diffuseFalloff;
				specularValueF = li.specular.rgb * specularValue * diffuseFalloff;
			}
		}

		totalAmbiant += ambiantValueF;
		totalDiffuse += diffuseValueF;
		totalSpecular += specularValueF;

        continue;

		// SHADOWS
		float4 posInMap = input.PosInMap[i];
		// validate that x & y are in [-1, 1]

        if (posInMap.x <= -1 || posInMap.x >= 0.9982 || posInMap.y <= -1 || posInMap.y >= 0.998)
        {
			totalDiffuse += diffuseValueF;
			totalSpecular += specularValueF;
			continue;
		}

		// Texture coordinates are 0-1
		float2 uv = float2((posInMap.x / 2) + 0.5f, (-posInMap.y / 2) + 0.5f);

		float depthV = shadowTexture.Sample(ShadowMapSampler, uv).r;
		
		
		// clacul of the distance to the plan (ortho)
        float vectorX = input.worldPos.x - li.position.x;
        float vectorY = input.worldPos.y - li.position.y;
        float vectorZ = input.worldPos.z - li.position.z;

		// Calcul du produit scalaire entre le vecteur et la normale du plan
        float dotProduct = vectorX * li.direction.x + vectorY * li.direction.y + vectorZ * li.direction.z;

		// Calcul de la magnitude de la normale pour obtenir la distance (distance signée)
        float normalMagnitude = sqrt(li.direction.x * li.direction.x + li.direction.y * li.direction.y + li.direction.z * li.direction.z);
        float distance = dotProduct / normalMagnitude / 500.f; // 500.f = far dist of the shadowmap camera

        if (distance <= depthV) // Not in shadows
        {
            totalDiffuse += diffuseValueF;
            totalSpecular += specularValueF;
        }
		
	}

	// Échantillonner la couleur du pixel à partir de la texture
	float3 couleurTexture = float3(1, 1, 1);

	if (hasAlbedoTexture > 0)
    {
        // Échantillonner la couleur du pixel à partir de la texture
        couleurTexture = textureEntree.Sample(SampleState, input.coordTex).rgb;
    }

    float noiseV = (input.noiseValue + 0.7f) / 1.7f;

	float3 finalColor = couleurTexture.rgb * (totalAmbiant * vAMat.rgb + totalDiffuse * vDMat.rgb + totalSpecular * vSMat.rgb)
        * noiseV 
		+ float3(0.95f, 0.98f, 0.99f);

    finalColor *= 1.f * float3(0.8f, 1.f, 1.f);
    finalColor /= 1.2f;

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
	Out.Pos = mul(Pos, matWorldViewProj);

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
	FillMode = Solid;
	DepthClipEnable = true;
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
