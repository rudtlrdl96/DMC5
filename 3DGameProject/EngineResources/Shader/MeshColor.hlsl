#include "Transform.fx"
#include "Animation.fx"
#include "Light.fx"
#include "RenderBaseValue.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
};

struct Output
{
    // w������ ���ְ�
    // viewport ��ı��� �����Ͷ��������� �����ش�.
    float4 POSITION : SV_POSITION;
    float4 VIEWPOSITION : POSITION0;
    float4 WORLDPOSITION : POSITION1;
    float4 WVPPOSITION : POSITION5;
    float4 NORMAL : NORMAL;
};

// ������� ���ؼ� ���� �ݻ縦 �����ϰ� ����
// �� ���� ����ϴ� ������ ���� �������� ������ �ߴ�.
// ������ ������ ���� �ǹ̰� ����.
// ���̶�� �÷��� ���س��� ���� �ȴ�.

// �װ� ���ؽ� ���̴������ϸ� �װ� �������� �ϸ� �÷����̵�
// �װ� ���ؽ� ���̴������ϸ� �װ� ��ν��̵�
// �װ� �ȼ� ���̴������ϸ� �װ� �����̵�

// �׷���ī�忡�� �̷����°�.
Output MeshColor_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    float4 InputNormal = _Input.NORMAL;
    InputNormal.w = 0.0f;
    
    // ��ũ����ǥ�� �̴�.
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    NewOutPut.WORLDPOSITION = mul(InputPos, WorldMatrix);
    NewOutPut.WVPPOSITION = NewOutPut.POSITION;
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(InputNormal, WorldView);
    
    return NewOutPut;
}

struct ForwardOutPut
{
    float4 ColorTarget : SV_Target0;
    float4 PosTarget : SV_Target1;
};

ForwardOutPut MeshColor_PS(Output _Input)
{
    ForwardOutPut Result;
    
    Result.ColorTarget = BaseColor;
    Result.PosTarget = _Input.WORLDPOSITION;
    
    Result.ColorTarget += AddColor;
    Result.ColorTarget *= MulColor;
    
    // ��ǻ�� ����Ʈ
    
    if (Result.ColorTarget.a <= 0.0f)
    {
        clip(-1);
    }
    
    float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, _Input.NORMAL, AllLight[0]);
    float4 SpacularRatio = CalSpacularLight(_Input.VIEWPOSITION, _Input.NORMAL, AllLight[0]);;
    float4 AmbientRatio = CalAmbientLight(AllLight[0]);
    
    
    float A = Result.ColorTarget.w;
    Result.ColorTarget = Result.ColorTarget * (DiffuseRatio + SpacularRatio + AmbientRatio);
    Result.ColorTarget.a = A;
    // Color += AllLight[0].LightColor;
    
    return Result;
}

