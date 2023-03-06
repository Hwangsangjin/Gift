struct VS_INPUT
{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

struct VPS_INOUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

cbuffer ConstantData : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 projection;
}

VPS_INOUTPUT vsmain(VPS_INOUTPUT input)
{
	VPS_INOUTPUT output = (VPS_INOUTPUT) 0;
	
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.texcoord = input.texcoord;
	
	return output;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(VPS_INOUTPUT input) : SV_TARGET
{
	return Color.Sample(ColorSampler, input.texcoord);
}