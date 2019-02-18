cbuffer ConstantBuffer
{
    float4x4 matFinal;
}
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut basic_sprite_vs_main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
    VOut output;
	output.position = mul(matFinal, position);
    output.color = color;
	output.texcoord = texcoord;
    return output;
}