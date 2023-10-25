#include "Transform.fx"
#include "RenderBaseValue.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 VIEWPOSITION : POSITIONT0;
    float4 TEXCOORD : TEXCOORD;
};

Output MeshAniTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
        
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.POSITION = mul(InputPos, WorldView);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
        
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D NoiseTexture : register(t1);

SamplerState ENGINEBASE : register(s0);

struct ForwardOutPut
{
    float4 ColorTarget : SV_Target0;
    float4 PosTarget : SV_Target1;
};

ForwardOutPut MeshAniTexture_PS(Output _Input)
{
    ForwardOutPut Result;
    
    float2 UV = _Input.TEXCOORD.xy;
    float2 Dir = UV - float2(0.5f, 0.5f);
    
    float4 Noise = NoiseTexture.Sample(ENGINEBASE, UV);
    
    UV += sin(Dir * Noise.x);
    
    Result.ColorTarget = DiffuseTexture.Sample(ENGINEBASE, UV);
    Result.PosTarget = _Input.VIEWPOSITION;
        
    if (Result.ColorTarget.a <= 0.0f)
    {
        clip(-1);
    }
    
    return Result;
}
