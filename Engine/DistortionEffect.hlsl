Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

cbuffer CONSTANT : register(b0)
{
	float distortion_level;
}

float2 distort(float2 p)
{
	float theta = atan2(p.y, p.x);
	float radius = length(p);

	radius = pow(radius, distortion_level);
	
	p.x = radius * cos(theta);
	p.y = radius * sin(theta);
	
	return 0.5f * (p + 1.0f);
}

float4 main(PS_INPUT input) : SV_TARGET
{
	float2 xy = 2.0f * input.texcoord.xy - 1.0f;
	float2 uv;
	float d = length(xy);
	
	if (d < 1.0f)
	{
		uv = distort(xy);
	}
	else
	{
		uv = input.texcoord.xy;
	}
	
	float4 color = Texture.Sample(TextureSampler, uv);
	return color;
}