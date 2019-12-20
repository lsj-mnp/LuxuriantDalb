struct VS_INPUT
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_INPUT_INSTANCED
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD;

	float4 InstanceWorld0 : INSTANCE_WORLD0;
	float4 InstanceWorld1 : INSTANCE_WORLD1;
	float4 InstanceWorld2 : INSTANCE_WORLD2;
	float4 InstanceWorld3 : INSTANCE_WORLD3;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};