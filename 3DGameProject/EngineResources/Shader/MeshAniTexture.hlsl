#include "Transform.fx"
#include "Animation.fx"
#include "Light.fx"
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
    float4 VIEWPOSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
};


// 빛계산을 통해서 빛의 반사를 구현하고 나면
// 그 빛을 계산하는 시점에 따라서 예전에는 구분을 했다.
// 이제는 구분이 거의 의미가 없다.
// 빛이라는 컬러를 구해내는 것이 된다.

// 그걸 버텍스 쉐이더에서하면 그걸 점단위로 하면 플랫쉐이딩
// 그걸 버텍스 쉐이더에서하면 그걸 고로쉐이딩
// 그걸 픽셀 쉐이더에서하면 그걸 퐁쉐이딩

Output MeshAniTexture_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
    }
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(_Input.NORMAL, WorldView);
    
    _Input.TANGENT.w = 0.0f;
    NewOutPut.TANGENT = mul(_Input.TANGENT, WorldView);
    
    _Input.BINORMAL.w = 0.0f;
    NewOutPut.BINORMAL = mul(_Input.BINORMAL, WorldView);

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
SamplerState ENGINEBASE : register(s0);

float4 MeshAniTexture_PS(Output _Input) : SV_Target0
{
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    if (Color.x == ClipColor.x && Color.y == ClipColor.y && Color.z == ClipColor.z)
    {
        clip(-1);
    }
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    float4 ResultColor = Color;
    
    if (0 != IsLight)
    {
        float4 BumpNormal = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
        BumpNormal = (BumpNormal * 2.0f) - 1.0f;
        BumpNormal = (BumpNormal.z * _Input.NORMAL) + (BumpNormal.x * _Input.BINORMAL) + (BumpNormal.y * _Input.TANGENT);
              
        float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, BumpNormal, AllLight[0]);
        float4 SpacularRatio = CalSpacularLight(_Input.VIEWPOSITION, BumpNormal, AllLight[0]);
        float4 AmbientRatio = CalAmbientLight(AllLight[0]);
        
        float A = Color.w;
        ResultColor = Color * (DiffuseRatio + SpacularRatio + AmbientRatio);
        ResultColor.a = A;
    }
    
    return ResultColor;
}
