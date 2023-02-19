Texture2D EarthColor : register(t0);
sampler EarthColorSampler : register(s0);

Texture2D EarthSpecular : register(t1);
sampler EarthSpecularSampler : register(s1);

Texture2D Clouds : register(t2);
sampler CloudsSampler : register(s2);

Texture2D EarthNight : register(t3);
sampler EarthNightSampler : register(s3);

struct PS_INPUT
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
	float time;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 earth_color = EarthColor.Sample(EarthColorSampler, 1.0f - input.texcoord);
	float earth_specular = EarthSpecular.Sample(EarthSpecularSampler, 1.0f - input.texcoord).r;
	float clouds = Clouds.Sample(CloudsSampler, 1.0f - input.texcoord + float2(time / 100.0f, 0.0f)).r;
	float4 earth_night = EarthNight.Sample(EarthNightSampler, 1.0f - input.texcoord);
	
	// Ambient light
	float ka = 1.5f;
	float3 ia = float3(0.09f, 0.082f, 0.082f);
	ia *= earth_color.rgb;
	
	float3 ambient_light = ka * ia;
	
	// Diffuse light
	float kd = 0.7f;
	float3 id_day = float3(1.0f, 1.0f, 1.0f);
	id_day *= (earth_color.rgb + clouds);
	
	float3 id_night = float3(1.0f, 1.0f, 1.0f);
	id_night *= (earth_night.rgb + clouds * 0.3f);
	
	float amount_diffuse_light = dot(light_direction.xyz, input.normal);
	
	float3 id = lerp(id_night, id_day, (amount_diffuse_light + 1.0f) / 2.0f);
	
	float3 diffuse_light = kd * id;
	
	// Specular light
	float ks = earth_specular;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	float3 reflected_light = reflect(light_direction.xyz, input.normal);
	float shininess = 30.0f;
	float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.direction_to_camera)), shininess);
	
	float3 specular_light = ks * amount_specular_light * is;
	
	// Final light
	float3 final_light = ambient_light + diffuse_light + specular_light;
	
	return float4(final_light, 1.0f);
}