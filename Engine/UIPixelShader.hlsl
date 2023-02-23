Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

cbuffer CONSTANT : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 projection;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	return Texture.Sample(TextureSampler, input.texcoord);
}