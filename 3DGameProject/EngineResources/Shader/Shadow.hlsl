#include "Transform.fx"
#include "Animation.fx"
#include "RenderBaseValue.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
    float4 BLENDWEIGHT : BLENDWEIGHT;
    int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 Pos5 : POSITION5;
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
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.Pos5 = NewOutPut.POSITION;
    NewOutPut.Pos5.w = 1.0f;
    
    return NewOutPut;
}

float4 Shadow_PS(Output _Value) : SV_Target0
{
    // ���̰��� �ϼ���
    return float4(max(0, _Value.Pos5.z / _Value.Pos5.w), 0.0f, 0.0f, 1.0f);
}