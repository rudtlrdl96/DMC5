struct Input
{
	float4 Pos : POSITION;
	float4 UV : TEXCOORD;
};

struct OutPut
{
	float4 Pos : SV_Position;
	float4 UV : TEXCOORD;
};

OutPut DistortionEffect_VS(Input _Value)
{
	OutPut OutPutValue = (OutPut) 0;
	OutPutValue.Pos = _Value.Pos;
	OutPutValue.UV = _Value.UV;
    
	return OutPutValue;
}
    
Texture2D DiffuseTex : register(t0);
Texture2D MaskTex : register(t1);
SamplerState WRAPSAMPLER : register(s0);

cbuffer DistortionOption : register(b2)
{
    float4 ScreenSize;
    float4 DistortionValue;
};

float4 DistortionEffect_PS(OutPut _Value) : SV_Target0
{
    float2 UV = _Value.UV.xy;
    float4 MaskColor = MaskTex.Sample(WRAPSAMPLER, UV);
	
    if (0 != MaskColor.a)
    {
        UV.x += sin(MaskColor.x) / ScreenSize.x * DistortionValue.x * MaskColor.a;
        UV.y += cos(MaskColor.x) / ScreenSize.y * DistortionValue.y * MaskColor.a;
    }
	
    float4 ResultColor = DiffuseTex.Sample(WRAPSAMPLER, saturate(UV));
    	
	return ResultColor;
}
