Texture2D Texture : register(t0);
SamplerState ss : register(s0); 
float4 basic_sprite_ps_main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * Texture.Sample(ss, texcoord);
}