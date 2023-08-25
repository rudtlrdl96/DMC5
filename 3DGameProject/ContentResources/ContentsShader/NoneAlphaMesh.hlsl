#include "Transform.fx"
#include "Light.fx"

cbuffer RenderBaseValue : register(b10)
{
    float DeltaTime = 0.0f;
    float SumDeltaTime = 0.0f;
    int IsAnimation = 0;
    int IsLight = 0;
    int IsNormal = 0;
    float4 ScreenScale;
    float4 Mouse;
};

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
};

Output MeshAniTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
        
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(_Input.NORMAL, WorldView);

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
SamplerState ENGINEBASE : register(s0);

float4 MeshAniTexture_PS(Output _Input) : SV_Target0
{
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    Color.a = 1.0f;
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    float4 ResultColor = Color;
    
    if (0 != IsLight)
    {
        float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, _Input.NORMAL, AllLight[0]);
        float4 SpacularRatio;
        float4 AmbientRatio = CalAmbientLight(AllLight[0]);
        
        float A = Color.w;
        ResultColor = (Color * DiffuseRatio) + (Color * AmbientRatio);
        ResultColor.a = A;
    }
    
    return ResultColor;
}
