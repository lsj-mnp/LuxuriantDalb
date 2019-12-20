#include "iDefault.hlsli"

cbuffer CBSpace : register(b0)
{
	float4x4 World;
	float4x4 Projection;
	float4x4 WorldProjection;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//Output.Position = mul(Input.Position, World);
	Output.Position = mul(Input.Position, WorldProjection);
	Output.TexCoord = Input.TexCoord;

	return Output;
}

VS_OUTPUT Instanced(VS_INPUT_INSTANCED Input)
{
	VS_OUTPUT Output;

	float4x4 InstanceWorld = float4x4(Input.InstanceWorld0, Input.InstanceWorld1, Input.InstanceWorld2, Input.InstanceWorld3);

	Output.Position = mul(Input.Position, InstanceWorld);
	Output.Position = mul(Output.Position, Projection);
	Output.TexCoord = Input.TexCoord;

	return Output;
}