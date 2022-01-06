// Description: Shader for post processing.

cbuffer SettingsBuffer : register (b0)
{
	bool useWaveDisplacement;
	bool useRandomStripeDisplacement;
	bool useChromaticAberration;
	bool useScanLines;
	float waveFrequency;
	float waveAmplitude;
	float stripesFrequency;
	float stripesThreshold;
	float stripeShiftAmount;
	float chromaticAberationAmount;
	float time;
};

Texture2D textureMap : register(t0);
Texture2D bloomMap : register(t1);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float random(float2 input) 
{
	return frac(sin(dot(input, float2(12.9898, 78.233))) * 43758.5453123);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float shift = 0.0f;
	float weight = 1.0f;

	if (useScanLines)
		weight = abs(sin(input.texCoord.y * 750.0f + time * 10.0f));

	if (useWaveDisplacement)
		shift = sin((input.texCoord.y + time) * waveFrequency) * waveAmplitude;

	if (useRandomStripeDisplacement)
	{
		float randomvalue = random(floor((input.texCoord.y + floor(time * 5.0f)) * stripesFrequency));
		float left = step(stripesThreshold, randomvalue);
		float right = step(randomvalue, 1.0f - stripesThreshold);
		if (left == 1.0f)
			shift += stripeShiftAmount;
		else if (right == 1.0f)
			shift -= stripeShiftAmount;
	}

	if (useChromaticAberration)
	{
		float amount = chromaticAberationAmount;
		float r = textureMap.Sample(textureSampler, float2(input.texCoord.x + shift - amount, input.texCoord.y - amount)).r;
		float g = textureMap.Sample(textureSampler, float2(input.texCoord.x + shift, input.texCoord.y)).g;
		float b = textureMap.Sample(textureSampler, float2(input.texCoord.x + shift + amount, input.texCoord.y + amount)).b;

		return float4(r, g, b, 1.0f) * weight;
	}
	else
	{
		return textureMap.Sample(textureSampler, float2(input.texCoord.x + shift, input.texCoord.y)) * weight +
			bloomMap.Sample(textureSampler, float2(input.texCoord.x + shift, input.texCoord.y)) * weight;
	}
}