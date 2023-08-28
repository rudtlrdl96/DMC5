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

OutPut JudgementCut_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    OutPutValue.Pos = _Value.Pos;
    OutPutValue.UV = _Value.UV;
    
    return OutPutValue;
}

cbuffer JudgementCutValue : register(b0)
{
    float4 Lines[32];
    int LineCount;    
    float LineThickness;
    float LineInter;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float GetXp(float4 Pos, float2 uv)
{
    float2 v1 = float2(Pos.z, Pos.w) - float2(Pos.x, Pos.y);
    float2 v2 = float2(Pos.z, Pos.w) - uv;
    return v1.x * v2.y - v1.y * v2.x;
}

float4 GetColor(float2 uv, float xp, float lineThickness, float4 inColor)
{
    if (uv.y < xp + lineThickness && uv.y > xp - lineThickness)
    {
        float4 pixel1 = DiffuseTex.Sample(WRAPSAMPLER, float2(uv.x, uv.y + lineThickness / 100.0));
        float4 pixel2 = DiffuseTex.Sample(WRAPSAMPLER, float2(uv.x, uv.y - lineThickness / 100.0));
        
        return float4(
        lerp(pixel1.x, pixel2.x, 0.1),
        lerp(pixel1.y, pixel2.y, 0.1),
        lerp(pixel1.z, pixel2.z, 0.1),
        lerp(pixel1.w, pixel2.w, 0.1)
        );
        
    }
    else
    {
        return inColor;
    }
}

float4 JudgementCut_PS(OutPut _Value) : SV_Target0
{    
    float2 uv = _Value.UV.xy;
    float2 resUv = float2(uv);
            
    float Offset = min(0.002 * SumDeltaTime.x, 0.02);
    float XP[32];
        
    for (int i = 0; i < LineCount; ++i)
    {
        float4 Line = Lines[i];
        
        float LineInterTime = i * LineInter;
        
        if (LineInterTime > SumDeltaTime.x)
        {
            continue;
        }
                
        if (0 == (i % 2))
        {
            Line += 0.005f * SumDeltaTime.x;
        }
        else
        {
            Line -= 0.009f * SumDeltaTime.x;
        }
                        
        XP[i] = GetXp(Line, uv);
        
        if (0 == (i % 2))
        {
            if (XP[i] >= 0.0)
            {
                resUv.x -= Offset;
            }
        }
        else
        {
            if (XP[i] <= 0.0)
            {              
                resUv.x -= Offset;
            }
        }
    }
    
    float4 TexColor = DiffuseTex.Sample(WRAPSAMPLER, resUv);
            
    for (int j = 0; j < LineCount; ++j)
    {
        float LineInterTime = j * LineInter;
        
        if (LineInterTime > SumDeltaTime.x)
        {
            continue;
        }
        
        TexColor = GetColor(resUv, ScreenScale.y * XP[j], LineThickness, TexColor);
    }
    
    //TexColor.a = 1.0f;
    
    return TexColor;
}
