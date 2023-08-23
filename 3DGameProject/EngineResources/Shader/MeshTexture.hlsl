#include "Transform.fx"


struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 BLENDWEIGHT : BLENDWEIGHT;
    int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct AniMat
{
    float4x4 Mat;
};

// �׷���ī�忡�� �̷����°�.
Output MeshTexture_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    
    NewOutPut.POSITION = _Input.POSITION;
    NewOutPut.POSITION.w = 1.0f;
    // �ڽ��� ���ð������� �ִϸ��̼��� ��Ű��
    // NewOutPut.POSITION = mul(_Input.POSITION, ArrAniMationMatrix[_Input.BLENDINDICES[0]].Mat);
    NewOutPut.POSITION = mul(NewOutPut.POSITION, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
SamplerState WRAPSAMPLER : register(s0);

float4 MeshTexture_PS(Output _Input) : SV_Target0
{
    float4 Color = DiffuseTexture.Sample(WRAPSAMPLER, _Input.TEXCOORD.xy);
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    return Color;
}
