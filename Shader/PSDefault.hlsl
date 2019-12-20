#include "iDefault.hlsli"

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 main(VS_OUTPUT Input) : SV_TARGET
{
	return Texture.Sample(Sampler, Input.TexCoord);
}

float4 Colorize(VS_OUTPUT Input) : SV_TARGET
{
	float4 Color = Texture.Sample(Sampler, Input.TexCoord);

	float u = (float)Input.Position.x / 1280.0;
	float v = (float)Input.Position.y / 720.0;

	Color.r *= sin(u * 3.14) * 0.5 + 0.5;
	Color.g *= sin(v * 3.14) * 0.5 + 0.5;
	Color.b *= sin(u * 3.14) * 0.5 + 0.5;

	return Color;
}