#include "Reflection.fx"

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

OutPut Merge_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

cbuffer CutData : register(b0)
{
    float CutStartX;
    float CutEndX;
    float CutStartY;
    float CutEndY;
};

float4 Merge_PS(OutPut _Value) : SV_Target0
{
    float2 UV = _Value.UV.xy;
    
    if (UV.x < CutStartX || UV.x > CutEndX)
    {
        clip(-1);
    }
    
    if (UV.y < CutStartY || UV.y > CutEndY)
    {
        clip(-1);
    }
    
    UV.x = map(UV.x, 0.0f, 1.0f, CutStartX, CutEndX);
    UV.y = map(UV.y, 0.0f, 1.0f, CutStartY, CutEndY);
    
    float4 Color = DiffuseTex.Sample(WRAPSAMPLER, UV);
       
    Color.a = saturate(Color.a);
    
    return Color;
}