cbuffer ConstantBuffer
{
    float4x4 matFinal;
	float	 alpha;
}
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut basic_draw_vs_main(float4 position : SV_POSITION, float4 color : COLOR)
{
    VOut output;
	output.position = mul(matFinal, position);
    output.color = color * float4(1.0, 1.0, 1.0, alpha);
    return output;
}