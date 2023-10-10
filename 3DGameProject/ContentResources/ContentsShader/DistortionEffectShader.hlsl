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
SamplerState CLAMPSAMPLER : register(s0);

cbuffer DistortionOption : register(b2)
{
    float4 ScreenSize;
    float4 DistortionValue;
};

float4 DistortionEffect_PS(OutPut _Value) : SV_Target0
{
    float2 UV = _Value.UV.xy;
    float4 MaskColor = MaskTex.Sample(CLAMPSAMPLER, UV);
	
    if (0 != MaskColor.a)
    {
        UV.x += cos(MaskColor.x) / ScreenSize.x * DistortionValue.x * max(0.2, MaskColor.a);
        UV.y += sin(MaskColor.x) / ScreenSize.y * DistortionValue.y * max(0.2, MaskColor.a);
    }
	    
    float4 ResultColor = DiffuseTex.Sample(CLAMPSAMPLER, UV);
    	
	return ResultColor;
}
