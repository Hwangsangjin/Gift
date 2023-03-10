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

cbuffer ConstantData : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 projection;
	float4 camera_position;
	LightData light;
	TerrainData terrain;
}

Texture2D HeightMap : register(t0);
sampler HeightMapSampler : register(s0);

Texture2D GroundMap : register(t1);
sampler GroundMapSampler : register(s1);

Texture2D CliffMap : register(t2);
sampler CliffMapSampler : register(s2);

VPS_INOUTPUT vsmain(VS_INPUT input)
{
	VPS_INOUTPUT output = (VPS_INOUTPUT) 0;
	float texel_size = 1.0f / terrain.height_map_size;
	
	float height = HeightMap.SampleLevel(HeightMapSampler, input.texcoord, 0.0f).r;
	output.position = mul(float4(input.position.x * terrain.size.x, height * terrain.size.y, input.position.z * terrain.size.z, 1.0f), 
	world);
	
	output.world_position = output.position.xyz;
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3) world));
	
	float t = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x, input.texcoord.y - texel_size), 0.0f).r;
	float b = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x, input.texcoord.y + texel_size), 0.0f).r;
	float l = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x - texel_size, input.texcoord.y), 0.0f).r;
	float r = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x + texel_size, input.texcoord.y), 0.0f).r;
	
	t *= terrain.size.y;
	b *= terrain.size.y;
	l *= terrain.size.y;
	r *= terrain.size.y;
	
	float3 normal = float3((r - l) * 0.5f, 1.0f, (b - t) * 0.5f);
	output.normal = normalize(mul(normal, (float3x3) world));
	
	return output;
}

float4 psmain(VPS_INOUTPUT input) : SV_TARGET
{
	float4 ground = GroundMap.Sample(GroundMapSampler, input.texcoord * 100.0f);
	float4 cliff = CliffMap.Sample(CliffMapSampler, input.texcoord * 60.0f);
	
	float4 color = ground;
	float angle = abs(dot(input.normal, float3(0.0f, 1.0f, 0.0f)));
	
	float min_angle = 0.5f;
	float max_angle = 1.0f;
	
	if (angle >= min_angle && angle <= max_angle)
		color = lerp(cliff, color, (angle - min_angle) * (1.0f / (max_angle - min_angle)));
	
	if (angle < min_angle)
		color = cliff;
	
	float3 light_direction = normalize(float3(light.direction.xyz));
	float3 direction_to_camera = normalize(input.world_position - camera_position.xyz);
	
	// Ambient light
	float ka = 4.0f;
	float3 ia = float3(0.09f, 0.09f, 0.09f);
	ia *= color.rgb;
	
	float3 ambient_light = ka * ia;
	
	// Diffuse light
	float kd = 0.7f;
	float3 id = light.color.rgb;
	id *= color.rgb;
	float amount_diffuse_light = max(dot(light_direction.xyz, input.normal), 0.0f);
	
	float3 diffuse_light = kd * id * amount_diffuse_light;
	
	// Specular light
	float ks = 0.0f;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	float3 reflected_light = reflect(light_direction.xyz, input.normal);
	float shininess = 30.0f;
	float amount_specular_light = pow(max(0.0f, dot(reflected_light, direction_to_camera)), shininess);
	
	float3 specular_light = ks * amount_specular_light * is;
	
	// Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;
	
	return float4(final_light, 1.0f);
}