struct VS_INPUT
{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

cbuffer CONSTANT : register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 projection;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	output.texcoord = input.texcoord;
	
	return output;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	return Color.Sample(ColorSampler, input.texcoord);
	//float3 light_direction = normalize(float3(-1.0f, 1.0f, 1.0f));
	
	//// Ambient light
	//float ka = 8.5f;
	//float3 ia = float3(0.09f, 0.082f, 0.082f);
	//ia *= color.rgb;
	
	//float3 ambient_light = ka * ia;
	
	//// Diffuse light
	//float kd = 0.7f;
	//float3 id = float3(1.0f, 1.0f, 1.0f);
	//id *= color.rgb;
	//float amount_diffuse_light = max(dot(light_direction.xyz, input.normal), 0.0f);
	
	//float3 diffuse_light = kd * id * amount_diffuse_light;
	
	//// Specular light
	////float ks = 0.0f;
	////float3 is = float3(1.0f, 1.0f, 1.0f);
	////float3 reflected_light = reflect(light_direction.xyz, input.normal);
	////float shininess = 30.0f;
	////float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.direction_to_camera)), shininess);
	
	////float3 specular_light = ks * amount_specular_light * is;
	
	//// Final light
	//float3 final_light = ambient_light + diffuse_light /*+ specular_light*/;
	
	//return float4(final_light, 1.0f);
}