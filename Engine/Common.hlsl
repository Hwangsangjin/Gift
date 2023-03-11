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
	float3 normal : NORMAL0;
	float3 world_position : TEXCOORD1;
};

struct CameraData
{
	row_major float4x4 view;
	row_major float4x4 projection;
	float4 position;
};

struct LightData
{
	float4 color;
	float4 direction;
};

struct TerrainData
{
	float4 size;
	float height_map_size;
};

struct WaterData
{
	float4 size;
	float height_map_size;
};

cbuffer ConstantData : register(b0)
{
	row_major float4x4 world;
	float time;
	CameraData camera;
	LightData light;
	TerrainData terrain;
	WaterData water;
};

float3 ComputePhongLighting
(
	CameraData camera,
	LightData light,
	float3 world_position,
	float3 normal,
	
	float ka,
	float3 ia,

	float kd,
	float3 id,

	float ks,
	float3 is,

	float shininess
)
{
	float3 light_direction = -normalize(float3(light.direction.xyz));
	float3 direction_to_camera = normalize(world_position - camera.position.xyz);
	
	// Ambient light	
	float3 ambient_light = ka * ia;
	
	// Diffuse light
	float amount_diffuse_light = max(dot(light_direction.xyz, normal), 0.0f);
	float3 diffuse_light = kd * (light.color.rgb * id) * amount_diffuse_light;
	
	// Specular light
	float3 reflected_light = reflect(light_direction.xyz, normal);
	float amount_specular_light = pow(max(0.0f, dot(reflected_light, direction_to_camera)), shininess);
	
	float3 specular_light = ks * amount_specular_light * is;
	
	// Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;
	
	return final_light;
}

float3 ComputeNormalFromHeightMap
(
	Texture2D HeightMap,
	sampler HeightMapSampler,
	float height_map_size,
	float2 texcoord,
	float normal_factor
)
{
	float texel_size = 1.0f / height_map_size;
	
	float t = HeightMap.SampleLevel(HeightMapSampler, float2(texcoord.x, texcoord.y - texel_size), 0.0f).r;
	float b = HeightMap.SampleLevel(HeightMapSampler, float2(texcoord.x, texcoord.y + texel_size), 0.0f).r;
	float l = HeightMap.SampleLevel(HeightMapSampler, float2(texcoord.x - texel_size, texcoord.y), 0.0f).r;
	float r = HeightMap.SampleLevel(HeightMapSampler, float2(texcoord.x + texel_size, texcoord.y), 0.0f).r;
	
	t *= normal_factor;
	b *= normal_factor;
	l *= normal_factor;
	r *= normal_factor;
	
	float3 normal = float3(-(r - l) * 0.5f, 1.0f, -(b - t) * 0.5f);
	
	return normalize(normal);

}