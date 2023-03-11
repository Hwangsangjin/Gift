#include "Common.hlsl"

Texture2D HeightMap : register(t0);
sampler HeightMapSampler : register(s0);

VPS_INOUTPUT vsmain(VS_INPUT input)
{
	VPS_INOUTPUT output = (VPS_INOUTPUT) 0;
	
	float2 texcoord1 = (input.texcoord * 1.0f) + time * 0.009f;
	float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 1.0f) + time * 0.01f;
	
	float height1 = HeightMap.SampleLevel(HeightMapSampler, texcoord1, 0.0f).r;
	float height2 = HeightMap.SampleLevel(HeightMapSampler, texcoord2, 0.0f).r;
	
	float height = lerp(height1, height2, 0.5f);
	
	output.position = mul(float4(input.position.x * water.size.x, height * water.size.y, input.position.z * water.size.z, 1.0f),
	world);
	
	output.world_position = output.position.xyz;
	output.position = mul(output.position, camera.view);
	output.position = mul(output.position, camera.projection);
	output.texcoord = input.texcoord;
	
	return output;
}

float4 psmain(VPS_INOUTPUT input) : SV_TARGET
{
	float2 texcoord1 = (input.texcoord * 8.0f) + time * 0.009f;
	float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 8.0f) + time * 0.01f;
	
	float3 normal1 = ComputeNormalFromHeightMap
	(
		HeightMap,
		HeightMapSampler,
		water.height_map_size,
		texcoord1,
		water.size.y * 3.0f
	);
	
	float3 normal2 = ComputeNormalFromHeightMap
	(
		HeightMap,
		HeightMapSampler,
		water.height_map_size,
		texcoord2,
		water.size.y * 3.0f
	);
	
	float3 normal = normalize(lerp(normal1, normal2, 0.5f));
	normal = normalize(mul(normal, (float3x3) world));
	
	float4 water = float4(0.24f, 0.37f, 0.49f, 1.0f);
	float4 crest = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float4 color = water;
	float angle = abs(dot(normal, float3(0.0f, 1.0f, 0.0f)));
	
	float min_angle = 0.5f;
	float max_angle = 1.0f;
	
	if (angle >= min_angle && angle <= max_angle)
		color = lerp(crest, color, (angle - min_angle) * (1.0f / (max_angle - min_angle)));
	
	if (angle < min_angle)
		color = crest;
	
	float3 final_light = ComputePhongLighting
	(
		camera,
		light,
		input.world_position.xyz,
		normal.xyz,
	
		3,
		color.rgb * float3(0.09f, 0.09f, 0.09f),
	
		0.7f,
		color.rgb,
	
		1.0f,
		float3(1.0f, 1.0f, 1.0f),
	
		30.0f
	);
	
	return float4(final_light, 0.8f);
}