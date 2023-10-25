#include "RenderBaseValue.fx"

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

OutPut MotionBlur_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}

Texture2D DiffuseTex : register(t0);
Texture2D PrevPosTex : register(t1);
Texture2D CurPosTex : register(t2);

SamplerState ENGINEBASE : register(s0);

#define BlurSampleCount 2

float4 MotionBlur_PS(OutPut _Value) : SV_Target0
{    
    float2 uv = _Value.UV.xy;
    
    float4 PreviousPos = PrevPosTex.Sample(ENGINEBASE, uv);
    float4 CurrentPos = CurPosTex.Sample(ENGINEBASE, uv);
    
    float2 Velocity = (CurrentPos - PreviousPos) / 10.0f;
    
    Velocity.x /= ScreenScale.x;
    Velocity.y /= ScreenScale.y;
        
    // Get the initial color at this pixel.    
    float4 TexColor = DiffuseTex.Sample(ENGINEBASE, uv);
    float Alpha = TexColor.a;
    
    uv += Velocity;
    
    for (int i = 1; i < BlurSampleCount; ++i)
    { 
        float4 CurrentColor = DiffuseTex.Sample(ENGINEBASE, uv);
        TexColor += CurrentColor;
        uv += Velocity;
    } 
    
    float4 FinalColor = TexColor / BlurSampleCount;
    FinalColor.a = Alpha;
    
    
    return FinalColor;
}
