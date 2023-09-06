#include "Light.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct Output
{
    float4 POSITION : SV_Position;
    float4 TEXCOORD : TEXCOORD;
};

// ������� ���ؼ� ���� �ݻ縦 �����ϰ� ����
// �� ���� ����ϴ� ������ ���� �������� ������ �ߴ�.
// ������ ������ ���� �ǹ̰� ����.
// ���̶�� �÷��� ���س��� ���� �ȴ�.

// �װ� ���ؽ� ���̴������ϸ� �װ� �������� �ϸ� �÷����̵�
// �װ� ���ؽ� ���̴������ϸ� �װ� ��ν��̵�
// �װ� �ȼ� ���̴������ϸ� �װ� �����̵�

// �׷���ī�忡�� �̷����°�.
Output DeferredMerge_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.POSITION = _Input.POSITION;
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    return NewOutPut;
}

struct OutPutColor
{
    float4 Result : SV_Target0;
};

Texture2D DifColor : register(t0);
Texture2D DifLight : register(t1);
Texture2D SpcLight : register(t2);
Texture2D AmbLight : register(t3);
SamplerState POINTWRAP : register(s0);

OutPutColor DeferredMerge_PS(Output _Input) : SV_Target0
{
 
    OutPutColor NewOutPut = (OutPutColor) 0;
    
    float4 Color = DifColor.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    
    if (0 >= Color.a)
    {
        clip(-1);
    }
    
    float4 DiffuseRatio = DifLight.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    float4 SpacularRatio = SpcLight.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    float4 AmbientRatio = AmbLight.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    
    NewOutPut.Result = Color * (DiffuseRatio + SpacularRatio + AmbientRatio);
    NewOutPut.Result.a = saturate(NewOutPut.Result.a);
    
    return NewOutPut;

    //NewOutPut.DifLight = float4(1.0f, 0.0f, 0.0f, 1.0f);
    //NewOutPut.SpcLight = float4(0.0f, 1.0f, 0.0f, 1.0f);
    //NewOutPut.AmbLight = float4(0.0f, 0.0f, 1.0f, 1.0f);
    
    // return NewOutPut;

}

