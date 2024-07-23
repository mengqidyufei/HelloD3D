struct VS_OUTPUT
{
	float3 color : Color;
	float4 pos: SV_POSITION;
};

VS_OUTPUT main(float2 pos : POSITION, float3 color : Color)
{
	VS_OUTPUT str;
	str.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	str.color = color;
	return str;
}