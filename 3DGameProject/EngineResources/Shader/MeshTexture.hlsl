#include "Transform.fx"
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
    float4 VIEWPOSITION : POSITION0;
    float4 WORLDPOSITION : POSITION1;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
};

Output MeshAniTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
        
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    NewOutPut.WORLDPOSITION = mul(InputPos, WorldMatrix);
        
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(_Input.NORMAL, WorldView);
    
    
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);

SamplerState ENGINEBASE : register(s0);

struct ForwardOutPut
{
    float4 ColorTarget : SV_Target0;
    float4 PosTarget : SV_Target1;
};

ForwardOutPut MeshAniTexture_PS(Output _Input)
{
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
        
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }

    ForwardOutPut Result;
    
    Result.ColorTarget = Color;
    Result.PosTarget = _Input.WORLDPOSITION;
    
    if (0 != IsLight)
    {
        float4 NormalDir = _Input.NORMAL;
        
        if (0 != IsNormal)
        {
            float4 BumpNormal = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
            BumpNormal = (BumpNormal * 2.0f) - 1.0f;
            BumpNormal = (BumpNormal.z * _Input.NORMAL) + (BumpNormal.x * _Input.BINORMAL) + (BumpNormal.y * -_Input.TANGENT);
        
            NormalDir = BumpNormal;
        }
        
        float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, NormalDir, AllLight[0]);
        float4 SpacularRatio = CalSpacularLight(_Input.VIEWPOSITION, NormalDir, AllLight[0]);
        float4 AmbientRatio = CalAmbientLight(AllLight[0]);
        
        float A = Color.w;
        Result.ColorTarget = Color * (DiffuseRatio + SpacularRatio + AmbientRatio);
        Result.ColorTarget.a = A;
    }
    
    return Result;
}
