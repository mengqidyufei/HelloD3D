struct VS_OUTPUT
{
	float3 color : Color;
	float4 pos: SV_POSITION;
};

cbuffer CBUF
{
	row_major matrix rotationZ;
};

VS_OUTPUT main(float2 pos : POSITION, float3 color : Color)
{
	VS_OUTPUT str;
	str.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), rotationZ);
	str.color = color;
	return str;
}