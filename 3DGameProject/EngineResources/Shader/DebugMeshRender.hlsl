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

OutPut Debug_VS(Input _Value)
{
    OutPut OutPutValue = (OutPut) 0;
    _Value.Pos.w = 1.0f;
    OutPutValue.Pos = mul(_Value.Pos, WorldViewProjectionMatrix);
    return OutPutValue;
}

cbuffer DebugColor : register(b0)
{
    float4 RenderColor;
}

float4 Debug_PS(OutPut _Value) : SV_Target0
{
    return RenderColor;
}