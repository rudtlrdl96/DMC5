#include "Transform.fx"

struct Input
{
    float4 Pos : POSITION;
    float4 UV : TEXCOORD;
};

struct OutPut
{
    float4 Pos : SV_Position;
    float4 UV : TEXCOORD0;
    float4 ClipUV : TEXCOORD1;
};


cbuffer AtlasData : register(b1)
{
    float2 FramePos;
    float2 FrameScale;
}

cbuffer ClipData : register(b2)
{
    float4 Clip;
}

cbuffer FlipData : register(b3)
{
    float4 Flip;
}

// 월드뷰프로젝션

OutPut Texture_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
	
    _Value.Pos.w = 1.0f;
    OutPutValue.Pos = mul(_Value.Pos, WorldViewProjectionMatrix);
    
    float4 VtxUV = _Value.UV;
    
    if (Flip.x != 0)
    {
        VtxUV.x = 1.0f - VtxUV.x;
    }
    
    if (Flip.y != 0)
    {
        VtxUV.y = 1.0f - VtxUV.y;
    }
    
    OutPutValue.UV.x = (VtxUV.x * FrameScale.x) + FramePos.x;
    OutPutValue.UV.y = (VtxUV.y * FrameScale.y) + FramePos.y;
    
    OutPutValue.ClipUV = _Value.UV;
    
    return OutPutValue;
}

cbuffer ColorOption : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
    float4 BSLCColor;
}

Texture2D DiffuseTex : register(t0);
SamplerState SAMPLER : register(s0);

struct OutColor
{
    float4 Color0 : SV_Target0;
    float4 Color1 : SV_Target1;
    float4 Color2 : SV_Target2;
    float4 Color3 : SV_Target3;
};

float4 Texture_PS(OutPut _Value) : SV_Target0
{
    float4 Color = DiffuseTex.Sample(SAMPLER, _Value.UV.xy);
    float saturation = HBSCColor.g * 2;
    float brightness = HBSCColor.b * 2 - 1;
    float contrast = HBSCColor.a * 2;
    
    if (Clip.z == 0)
    {
        if (_Value.ClipUV.x > Clip.x)
        {
            clip(-1);
        }
    }
    else
    {
        if (_Value.ClipUV.x < 1.0f - Clip.x)
        {
            clip(-1);
        }
    }
    
    if (Clip.w == 0)
    {
        if (_Value.ClipUV.y > Clip.y)
        {
            clip(-1);
        }
    }
    else
    {
        if (_Value.ClipUV.y < 1.0f - Clip.y)
        {
            clip(-1);
        }
    }
    
    Color *= MulColor;
    Color += PlusColor;
    
    Color.rgb = (Color.rgb - 0.5f) * contrast + 0.5f;
    Color.rgb = Color.rgb + brightness;

    float3 intensity = dot(Color.rgb, float3(0.39, 0.59, 0.11));
    Color.rgb = lerp(intensity, Color.rgb, saturation);

    return Color;
}