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
Texture2D MaskTex : register(t1);
Texture2D PrevPosTex : register(t2);
Texture2D CurPosTex : register(t3);

SamplerState ENGINEBASE : register(s0);

#define BlurSampleCount 6

cbuffer MotionBlurData : register(b0)
{
    float4x4 PrevFrameViewProjection;
};

float3 GetBlurColor(float2 _CurUV, float2 _MoveUV, Texture2D _MainTex, Texture2D _MaskTex, SamplerState _Sample, float _Ratio)
{
    float4 MaskColor = _MaskTex.Sample(_Sample, _MoveUV);    
    float4 ResultColor;
    
    if (0 != MaskColor.a)
    {
        ResultColor = _MainTex.Sample(_Sample, saturate(_CurUV));
    }
    else
    {
        ResultColor = _MainTex.Sample(_Sample, saturate(_MoveUV));
    }
    
    return ResultColor.rgb * _Ratio;
}

float4 MotionBlur_PS(OutPut _Value) : SV_Target0
{    
    float2 CurUV = _Value.UV.xy;
    float2 MoveUV = _Value.UV.xy;
      
    float4 PreviousPos = PrevPosTex.Sample(ENGINEBASE, CurUV);
    PreviousPos.w = 1.0f;
    PreviousPos = mul(PreviousPos, PrevFrameViewProjection);
    PreviousPos.xyz /= PreviousPos.w;
        
    float4 CurrentPos = CurPosTex.Sample(ENGINEBASE, CurUV);
    CurrentPos.w = 1.0f;
    CurrentPos = mul(CurrentPos, PrevFrameViewProjection);
    CurrentPos.xyz /= CurrentPos.w;
    
    float2 Velocity = (CurrentPos.xy - PreviousPos.xy) / (BlurSampleCount * 4.0f);
    
    //Velocity.x /= ScreenScale.x;
    //Velocity.y /= ScreenScale.y;
            
    // Get the initial color at this pixel.    
    float4 TexColor = DiffuseTex.Sample(ENGINEBASE, CurUV);
    TexColor.rgb *= 0.4f; 
    float Alpha = TexColor.a;
    
    float Ratio = 0.6f / BlurSampleCount;
    
    for (int i = 0; i < BlurSampleCount; ++i)
    {
        MoveUV += Velocity;
        TexColor.rgb += GetBlurColor(CurUV, MoveUV, DiffuseTex, MaskTex, ENGINEBASE, Ratio);
    }
    
    TexColor.a = Alpha;
    return TexColor;
}
