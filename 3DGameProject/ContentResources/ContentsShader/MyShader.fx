#include "Transform.fx"

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


cbuffer AtlasData : register(b1)
{
    // 0.0 0.5
    float2 FramePos;
    // 0.5 0.5 
    float2 FrameScale;
    // float4 AtlasUV;
}

// 월드뷰프로젝션

OutPut MyShader_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
	
    _Value.Pos.w = 1.0f;
    OutPutValue.Pos = mul(_Value.Pos, WorldViewProjectionMatrix);
    
    OutPutValue.UV.x = (_Value.UV.x * FrameScale.x) + FramePos.x;
    OutPutValue.UV.y = (_Value.UV.y * FrameScale.y) + FramePos.y;
    
    return OutPutValue;
}

cbuffer ColorOption : register(b0)
{
    float4 MulColor;
    float4 PlusColor;
}

cbuffer TestColor : register(b1)
{
    float4 AColor;
}

Texture2D DiffuseTex : register(t0);
SamplerState WRAPSAMPLER : register(s0);

struct OutColor
{
    float4 Color0 : SV_Target0;
    float4 Color1 : SV_Target1;
    float4 Color2 : SV_Target2;
    float4 Color3 : SV_Target3;
};

float4 MyShader_PS(OutPut _Value) : SV_Target0
{
    float4 Color = DiffuseTex.Sample(WRAPSAMPLER, _Value.UV.xy);
    
    Color *= MulColor;
    Color += PlusColor; 
    Color += AColor;
    
    return Color;
}