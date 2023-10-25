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
    // w나누기 해주고
    // viewport 행렬까지 레스터라이저에서 곱해준다.
    float4 POSITION : SV_POSITION;
    float4 VIEWPOSITION : POSITION0;
    float4 WVPPOSITION : POSITION5;
    float4 NORMAL : NORMAL;
};

// 빛계산을 통해서 빛의 반사를 구현하고 나면
// 그 빛을 계산하는 시점에 따라서 예전에는 구분을 했다.
// 이제는 구분이 거의 의미가 없다.
// 빛이라는 컬러를 구해내는 것이 된다.

// 그걸 버텍스 쉐이더에서하면 그걸 점단위로 하면 플랫쉐이딩
// 그걸 버텍스 쉐이더에서하면 그걸 고로쉐이딩
// 그걸 픽셀 쉐이더에서하면 그걸 퐁쉐이딩

// 그래픽카드에서 이뤄지는것.
Output MeshColor_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    float4 InputNormal = _Input.NORMAL;
    InputNormal.w = 0.0f;
    
    // 스크린좌표계 이다.
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
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
    Result.PosTarget = _Input.VIEWPOSITION;
    
    Result.ColorTarget += AddColor;
    Result.ColorTarget *= MulColor;
    
    // 디퓨즈 라이트
    
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

