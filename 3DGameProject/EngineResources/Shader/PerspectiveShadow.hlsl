#include "Transform.fx"
#include "Animation.fx"
#include "RenderBaseValue.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 BLENDWEIGHT : BLENDWEIGHT;
    int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 ShadowPos : POSITION;
};

Output Shadow_VS(Input _Value)
{
    Output NewOutPut = (Output) 0;
    float4 InputPos = _Value.POSITION;
    InputPos.w = 1.0f;
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Value.BLENDWEIGHT, _Value.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
    }
    
    NewOutPut.ShadowPos = mul(InputPos, WorldView);
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    
    return NewOutPut;
}

float4 Shadow_PS(Output _Value) : SV_Target0
{
    return float4(max(0.0f, length(_Value.ShadowPos.xyz)), 0.0f, 0.0f, 1.0f);
}