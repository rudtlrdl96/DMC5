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
    float4 MaxPixel;
};

float4 DistortionEffect_PS(OutPut _Value) : SV_Target0
{
    float2 UV = _Value.UV.xy;
    float4 MaskColor = MaskTex.Sample(CLAMPSAMPLER, UV);
	
    if (0 != MaskColor.a)
    {
        float PixelSizeX = MaxPixel.x / ScreenSize.x;
        float PixelSizeY = MaxPixel.y / ScreenSize.y;
        
        float AddUV_X = (cos(MaskColor.x * DistortionValue.x) * PixelSizeX) * max(0.1245f, MaskColor.a);
        float AddUV_Y = (sin(MaskColor.x * DistortionValue.y) * PixelSizeY) * max(0.213f, MaskColor.a);
        
        UV.x += AddUV_X;
        UV.y += AddUV_Y;
    }
	    
    UV.x = saturate(UV.x);
    UV.y = saturate(UV.y);
    
    float4 ResultColor = DiffuseTex.Sample(CLAMPSAMPLER, UV);
    	
	return ResultColor;
}
