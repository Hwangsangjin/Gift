#include "Common.hlsl"

Texture2D HeightMap : register(t0);
sampler HeightMapSampler : register(s0);

Texture2D GroundMap : register(t1);
sampler GroundMapSampler : register(s1);

Texture2D CliffMap : register(t2);
sampler CliffMapSampler : register(s2);

VPS_INOUTPUT vsmain(VS_INPUT input)
{
	VPS_INOUTPUT output = (VPS_INOUTPUT) 0;
	
	float height = HeightMap.SampleLevel(HeightMapSampler, input.texcoord, 0.0f).r;
	output.position = mul(float4(input.position.x * terrain.size.x, height * terrain.size.y, input.position.z * terrain.size.z, 1.0f),
	world);
	
	output.world_position = output.position.xyz;
	output.position = mul(output.position, camera.view);
	output.position = mul(output.position, camera.projection);
	output.texcoord = input.texcoord;

	return output;
}

float4 psmain(VPS_INOUTPUT input) : SV_TARGET
{
	float3 normal = ComputeNormalFromHeightMap
	(
		HeightMap,
		HeightMapSampler,
		terrain.height_map_size,
		input.texcoord,
		terrain.size.y
	);
	normal = normalize(mul(normal, (float3x3) world));
	
	float height = HeightMap.SampleLevel(HeightMapSampler, input.texcoord, 0.0f).r;
	float4 ground = GroundMap.Sample(GroundMapSampler, input.texcoord * 100.0f);
	float4 cliff = CliffMap.Sample(CliffMapSampler, input.texcoord * 60.0f);
	
	float4 color = ground;
	float angle = abs(dot(normal, float3(0.0f, 1.0f, 0.0f)));
	
	float min_angle = 0.5f;
	float max_angle = 1.0f;
	
	if (angle >= min_angle && angle <= max_angle)
		color = lerp(cliff, color, (angle - min_angle) * (1.0f / (max_angle - min_angle)));
	
	if (angle < min_angle)
		color = cliff;
	
	float alpha = 0.0;

	if (height < 0.01f)
		alpha = 0.0f;
	else
		alpha = 1.0f;
	
	float3 final_light = ComputePhongLighting
	(
		camera,
		light,
		input.world_position.xyz,
		normal.xyz,
	
		4,
		color.rgb * float3(0.09f, 0.09f, 0.09f),
	
		0.7f,
		color.rgb,
	
		0.0f,
		float3(1.0f, 1.0f, 1.0f),
	
		10.0f
	);
	
	return float4(final_light, alpha);
}