float3 mod289(float3 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float2 mod289(float2 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float3 permute(float3 x) {
    return mod289((x * 34.0 + 1.0) * x);
}

float3 taylorInvSqrt(float3 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}

// output noise is in range [-1, 1]
float snoise(float2 v) {
    const float4 C = float4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                            0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                            -0.577350269189626, // -1.0 + 2.0 * C.x
                            0.024390243902439); // 1.0 / 41.0

    // First corner
    float2 i  = floor(v + dot(v, C.yy));
    float2 x0 = v -   i + dot(i, C.xx);

    // Other corners
    float2 i1;
    i1.x = step(x0.y, x0.x);
    i1.y = 1.0 - i1.x;

    // x1 = x0 - i1  + 1.0 * C.xx;
    // x2 = x0 - 1.0 + 2.0 * C.xx;
    float2 x1 = x0 + C.xx - i1;
    float2 x2 = x0 + C.zz;

    // Permutations
    i = mod289(i); // Avoid truncation effects in permutation
    float3 p =
      permute(permute(i.y + float3(0.0, i1.y, 1.0))
                    + i.x + float3(0.0, i1.x, 1.0));

    float3 m = max(0.5 - float3(dot(x0, x0), dot(x1, x1), dot(x2, x2)), 0.0);
    m = m * m;
    m = m * m;

    // Gradients: 41 points uniformly over a line, mapped onto a diamond.
    // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
    float3 x = 2.0 * frac(p * C.www) - 1.0;
    float3 h = abs(x) - 0.5;
    float3 ox = floor(x + 0.5);
    float3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    m *= taylorInvSqrt(a0 * a0 + h * h);

    // Compute final noise value at P
    float3 g = float3(
        a0.x * x0.x + h.x * x0.y,
        a0.y * x1.x + h.y * x1.y,
        g.z = a0.z * x2.x + h.z * x2.y
    );
    return 130.0 * dot(m, g);
}

float snoise01(float2 v) {
    return snoise(v) * 0.5 + 0.5;
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
    float4x4 matViewProj; // la matrice totale 
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
	float4 position  : Position;
	float3 normal    : NORMAL;
    float3 binormal  : BINORMAL;
    float3 tangent   : TANGENT;
	float3 cameraVec : TEXCOORD0;
	float2 coordTex  : TEXCOORD1;
	float3 worldPos  : TEXCOORD2;
	float3 cameraDir : TEXCOORD3;
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


    /*ANCHOR
    float noiseV = snoise01(vPos.xz * 2.0f);
    vPos.y = vPos.y + noiseV * 1;

    output.noiseValue = noiseV;
    */

    output.position = mul(vPos, matWorldViewProj);
    output.worldPos = mul(vPos, matWorld);

    //output.worldPos = float3(worldPos.x, worldPos.y + noiseV, worldPos.z);

    output.coordTex = coordTex;

    output.cameraVec = vCamera.xyz - output.worldPos;
    output.cameraDir = dCamera.xyz;

    /*ANCHOR
	float4 PosInMaps[MAX_LIGHTS];

	for (uint i = 0; i < MAX_LIGHTS; ++i) {
		Light li = lights[i];
		
		if (!li.initialized) continue;
		
		float4 tempValue = mul(vPos, li.matWorldViewProj);
		PosInMaps[i] = tempValue;
	}

	output.PosInMap = PosInMaps;
    */

	return output;
}

struct HS_CONSTANTES_Sortie
{
    float edgeTesselationFactor[4]   : SV_TessFactor;
    float insideTesselationFactor[2] : SV_InsideTessFactor;
};

struct HS_Sortie
{
	float4 position  : Position;
	float3 normal    : NORMAL;
    float3 binormal  : BINORMAL;
    float3 tangent   : TANGENT;
	float3 cameraVec : TEXCOORD0;
	float2 coordTex  : TEXCOORD1;
	float3 worldPos  : TEXCOORD2;
	float3 cameraDir : TEXCOORD3;
};

#define TessellationFactor 1

HS_CONSTANTES_Sortie ConstantHS(InputPatch<VS_Sortie, 4> ip, uint PatchID :SV_PrimitiveID)
{
    HS_CONSTANTES_Sortie Sortie;
    
    Sortie.edgeTesselationFactor[0] = 
    Sortie.edgeTesselationFactor[1] = 
    Sortie.edgeTesselationFactor[2] = 
    Sortie.edgeTesselationFactor[3] = TessellationFactor;

    Sortie.insideTesselationFactor[0] = 
    Sortie.insideTesselationFactor[1] = TessellationFactor;

    return Sortie;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
HS_Sortie EssaiHS( InputPatch<VS_Sortie, 4> p, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID )
{
    HS_Sortie Sortie;
    
    Sortie.position = p[i].position;
    Sortie.normal = p[i].normal;
    Sortie.binormal = p[i].binormal;
    Sortie.tangent = p[i].tangent;
    Sortie.cameraDir = p[i].cameraDir;
    Sortie.cameraVec = p[i].cameraVec;
    Sortie.worldPos = p[i].worldPos;
    Sortie.coordTex = p[i].coordTex;

    return Sortie;
}

struct DS_Sortie
{
	float4 position  : Position;
	float3 normal    : NORMAL;
    float3 binormal  : BINORMAL;
    float3 tangent   : TANGENT;
	float3 cameraVec : TEXCOORD0;
	float2 coordTex  : TEXCOORD1;
	float3 worldPos  : TEXCOORD2;
	float3 cameraDir : TEXCOORD3;
};

[domain("quad")]
DS_Sortie EssaiDS( HS_CONSTANTES_Sortie entree, float2 UV : SV_DomainLocation, const OutputPatch<HS_Sortie, 4> quad ) 
{
    DS_Sortie Sortie;
    
    float3 verticalPos1 = lerp(quad[0].position, quad[1].position, UV.y);
    float3 verticalPos2 = lerp(quad[3].position, quad[2].position, UV.y);
    float3 finalPos = lerp(verticalPos1, verticalPos2, UV.x);
    
    Sortie.position = mul(float4(finalPos, 1), matViewProj);
    
    float3 coordTex1 = lerp(quad[0].coordTex,quad[1].coordTex,UV.y);
    float3 coordTex2 = lerp(quad[3].coordTex,quad[2].coordTex,UV.y);
    Sortie.coordTex = lerp(coordTex1, coordTex2,UV.x);
    
    return Sortie;
}




float4 MainPS(DS_Sortie input) : SV_Target
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

		if (li.lightType == 0) // ambiant
		{
			totalAmbiant += li.ambiant;
		}
		else if (li.lightType == 1) // Directionnal
		{
			float3 L = normalize(-li.direction.xyz);
			float3 diff = saturate(dot(N, L));
			//float3 R = normalize(2 * diff * N - L);
			//float3 S = pow(saturate(dot(R, V)), li.specularPower);

			float3 H = normalize(L + V);
			float NdotH = saturate(dot(N, H));
			float puissance = max(EPSILON, Ns * li.specularPower);
			float specularValue = pow(NdotH, puissance);

			totalAmbiant += li.ambiant;
			totalDiffuse += li.diffuse.rgb * diff;
			totalSpecular += li.specular.rgb * specularValue;
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

				totalAmbiant += li.ambiant.xyz * diffuseFalloff;
				totalDiffuse += li.diffuse.rgb * diff * diffuseFalloff;
				totalSpecular += li.specular.rgb * specularValue * diffuseFalloff;
			}

/*
			float3 toLight = li.position.xyz - input.position.xyz;
			float3 toEye = input.cameraVec.xyz - input.position.xyz;
			float distToLight = length(toLight);
			
			toLight = normalize(toLight);

			float NDotL = saturate(dot(toLight, N));
			totalDiffuse += li.diffuse.rgb * NDotL;
			
			toEye = normalize(toEye);
   			float3 HalfWay = normalize(toEye + toLight);
   			float NDotH = saturate(dot(HalfWay, N));
			float puissance = max(EPSILON, Ns * li.specularPower);
   			totalDiffuse += li.specular.rgb * pow(NDotH, puissance);

			float conAtt = saturate((cosAng - SpotCosOuterCone) * SpotCosInnerConeRcp);
   			conAtt *= conAtt;

			float DistToLightNorm = 1.0 - saturate(DistToLight * SpotLightRangeRcp);
		    float Attn = DistToLightNorm * DistToLightNorm;
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

    float noiseV = (input.noiseValue + 0.7f) / 1.7f;

	float3 finalColor = couleurTexture.rgb * 
        (totalAmbiant * vAMat.rgb + totalDiffuse * vDMat.rgb + totalSpecular * vSMat.rgb)
        * noiseV;        ;

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
