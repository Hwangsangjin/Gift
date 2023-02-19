struct VS_INPUT
{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 direction_to_camera : TEXCOORD1;
};

cbuffer CONSTANT : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 projection;
	float4 light_direction;
	float4 camera_position;
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	// World space
	output.position = mul(input.position, world);
	output.direction_to_camera = normalize(output.position.xyz - camera_position.xyz);
	// View space
	output.position = mul(output.position, view);
	// Projection space
	output.position = mul(output.position, projection);
	
	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}