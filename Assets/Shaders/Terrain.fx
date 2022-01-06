// Description: Shader for terrain rendering

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
	bool useShadowMap;
	float depthBias;
}

Texture2D diffuseMap : register(t0);
Texture2D shadowMap : register(t1);
SamplerState textureSampler :register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 worldNormal : NORMAL;
	float3 dirToLight : TEXCOORD0;
	float3 dirToView : TEXCOORD1;
	float2 texCoord : TEXCOORD2;
	float4 lightNDCPosition : TEXCOORD3;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float3 position = input.position;

	matrix toNDC = wvp[0];
	matrix toLightNDC = wvp[1];
	matrix toWorld = world;

	output.position = mul(float4(position, 1.0f), toNDC);
	output.worldNormal = mul(input.normal, (float3x3)(toWorld));
	output.dirToLight = -lightDirection;
	output.dirToView = normalize(viewPosition - mul(float4(position, 1.0f), toWorld).xyz);
	output.texCoord = input.texCoord;
	output.lightNDCPosition = mul(float4(position, 1.0f), toLightNDC);
	return output;
}

float4 PS(VS_OUTPUT input, bool isFrontFace : SV_isFrontFace) : SV_Target
{
	float3 n = normalize(input.worldNormal);
	float3 L = normalize(input.dirToLight);
	float3 v = normalize(input.dirToView);

	float4 ambient = lightAmbient * materialAmbient;

	float d = saturate(dot(L, n));
	float4 diffuse = d * lightDiffuse * materialDiffuse;

	float3 r = reflect(-L, n);
	float base = saturate(dot(r, v));
	float s = pow(base, materialPower);
	float4 specular = s * lightSpecular * materialSpecular;

	float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float specularColor = 1.0f;

	float4 color = (ambient + diffuse) * diffuseColor + (specular * specularColor);

	if (useShadowMap)
	{
		float actualDepth = 1.0f - input.lightNDCPosition.z / input.lightNDCPosition.w;
		float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
		shadowUV = shadowUV + 1.0f;
		shadowUV = shadowUV * 0.5f;
		shadowUV.y = 1.0f - shadowUV.y;

		if (saturate(shadowUV.x) == shadowUV.x &&
			saturate(shadowUV.y) == shadowUV.y)
		{
			float savedDepth = shadowMap.Sample(textureSampler, shadowUV).r;
			if (actualDepth + depthBias < savedDepth)
				color = ambient * diffuseColor;
		}
	}

	return color;
}