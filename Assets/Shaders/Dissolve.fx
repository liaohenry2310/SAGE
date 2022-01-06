// Description: Dissolve Shader.

cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix wvp[2];
	float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float materialPower;
}

cbuffer SettingsBuffer : register(b3)
{
	bool useSpecularMap; //bools are 4 bytes in HLSL
	bool useBumpMap;
	float bumpWeight;
	bool useNormalMap;
	bool useShadowMap;
	float depthBias;
	float dissolveAmount;
	float burnAmount;
	float4 color1;
	float4 color2;
	float4 color3;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D bumbMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D shadowMap : register(t4);
Texture2D noiseMap : register(t5);
SamplerState textureSampler :register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 worldTangent : TANGENT;
	float3 dirToLight : TEXCOORD0;
	float3 dirToView : TEXCOORD1;
	float2 texCoord : TEXCOORD2;
	float4 lightNDCPosition : TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	
	float3 position = input.position;
	if (useBumpMap)
	{
		float displacement = bumbMap.SampleLevel(textureSampler, input.texCoord, 0).r;
		position = position	+ (input.normal * displacement * bumpWeight);
	}

	output.position = mul(float4(position, 1.0f), wvp[0]);
	output.worldNormal = mul(input.normal, (float3x3)world);
	output.worldTangent = mul(input.tangent, (float3x3)world);
	output.dirToLight = -lightDirection;
	output.dirToView = normalize(viewPosition - mul(float4(position, 1.0f), world).xyz);
	output.texCoord = input.texCoord;
	output.lightNDCPosition = mul(float4(position, 1.0f), wvp[1]);
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 n = normalize(input.worldNormal);
	float3 t = normalize(input.worldTangent);
	float3 b = normalize(cross(n, t));
	float3 L = normalize(input.dirToLight);
	float3 v = normalize(input.dirToView);

	if (useNormalMap)
	{
		float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
		float3 unpackedNormal = normalize(float3((normalColor.xy * 2.0f) - 1.0f, normalColor.z));
		float3x3 tbnw = float3x3(t, b, n);
		n = mul(unpackedNormal, tbnw);
	}

	float4 ambient = lightAmbient * materialAmbient;

	float d = saturate(dot(L, n));
	float4 diffuse = d * lightDiffuse * materialDiffuse;
	
	float3 r = reflect(-L, n);
	float base = saturate(dot(r, v));
	float s = pow(base, materialPower);
	float4 specular = s * lightSpecular * materialSpecular;

	float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float specularColor = 1.0f;
	if (useSpecularMap)
		specularColor = specularMap.Sample(textureSampler, input.texCoord).r;

	float4 color = (ambient + diffuse) * diffuseColor + (specular * specularColor);

	if (useShadowMap)
	{
		float actualDepth = 1.0f - input.lightNDCPosition.z / input.lightNDCPosition.w;
		float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
		shadowUV = shadowUV + 1.0f;
		shadowUV = shadowUV * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;

		float savedDepth = shadowMap.Sample(textureSampler, shadowUV).r;
		if(actualDepth + depthBias < savedDepth)
			color = ambient * diffuseColor;
	}

	//Dissolve Effect
	float noiseValue = noiseMap.Sample(textureSampler, input.texCoord).r;
	if (noiseValue < dissolveAmount - burnAmount)
	{
		color.a = 0.0f;
	}
	if (noiseValue < dissolveAmount && noiseValue > dissolveAmount - burnAmount)
	{
		float burnValue = noiseValue - (dissolveAmount - burnAmount);
		burnValue = burnValue / burnAmount;

		if (burnValue < 0.5f)
			color = lerp(color1, color2, burnValue * 2.0f);
		else
			color = lerp(color2, color3, burnValue * 2.0f - 1.0f);
	}

	return color;
}