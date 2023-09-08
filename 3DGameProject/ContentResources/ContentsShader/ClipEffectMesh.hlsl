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

Output MeshTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
        
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    NewOutPut.WORLDPOSITION = mul(InputPos, WorldMatrix);
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(_Input.NORMAL, WorldView);
    
    _Input.TANGENT.w = 0.0f;
    NewOutPut.TANGENT = mul(_Input.TANGENT, WorldView);
    
    _Input.BINORMAL.w = 0.0f;
    NewOutPut.BINORMAL = mul(_Input.BINORMAL, WorldView);

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0); // ALBM
//Texture2D NormalTexture : register(t1); // NRMR
//Texture2D SpecularTexture : register(t2); // ATOS

SamplerState ENGINEBASE : register(s0);

struct DeferredOutPut
{
    float4 DifTarget : SV_Target0;
};

cbuffer ClipData : register(b0)
{
    float ClipStartX;
    float ClipEndX;
    
    float ClipStartY;
    float ClipEndY;
};

DeferredOutPut MeshTexture_PS(Output _Input)
{
    DeferredOutPut Result = (DeferredOutPut)0;
    
    float2 UV = _Input.TEXCOORD.xy;
    
    if (ClipStartX > UV.x || ClipEndX < UV.x)
    {
        clip(-1);
    }   
    
    if (ClipStartY > UV.y || ClipEndY < UV.y)
    {
        clip(-1);
    }
    
    Result.DifTarget = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
        
    return Result;
}