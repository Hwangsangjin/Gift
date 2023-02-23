Texture2D TextureColor : register(t0);
sampler TextureColorSampler : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 world_position : TEXCOORD1;
};

cbuffer CONSTANT : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 projection;
	float4 light_direction;
	float4 camera_position;
	float4 light_position;
	float light_radius;
	float time;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 texture_color = TextureColor.Sample(TextureColorSampler, float2(input.texcoord.x, 1.0f - input.texcoord.y));
	
	// Ambient light
	float ka = 1.5f;
	float3 ia = float3(0.09f, 0.082f, 0.082f);
	ia *= texture_color.rgb;
	
	float3 ambient_light = ka * ia;
	
	// Diffuse light
	float kd = 0.7f;
	float3 light_direction = normalize(light_position.xyz - input.world_position.xyz);
	float distance_light_object = length(light_position.xyz - input.world_position.xyz);
	
	float fade_area = max(0.0f, distance_light_object - light_radius);
	
	float constant_func = 1.0f;
	float linear_func = 2.0f;
	float quadratic_func = 2.0f;
	
	float attenuation = constant_func + linear_func * fade_area + quadratic_func * fade_area * fade_area;
	
	float amount_diffuse_light = max(0.0f, dot(light_direction.xyz, input.normal));
	float3 id = float3(1.0f, 1.0f, 1.0f);
	id *= texture_color.rgb;
	float3 diffuse_light = (kd * id * amount_diffuse_light) / attenuation;
	
	// Specular light
	float ks = 0.0f;
	float3 direction_to_camera = normalize(input.world_position.xyz - camera_position.xyz);
	float3 is = float3(1.0f, 1.0f, 1.0f);
	float3 reflected_light = reflect(light_direction.xyz, input.normal);
	float shininess = 30.0f;
	float amount_specular_light = pow(max(0.0f, dot(reflected_light, direction_to_camera)), shininess);
	
	float3 specular_light = (ks * amount_specular_light * is) / attenuation;
	
	// Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;
	
	return float4(final_light, 1.0f);
}