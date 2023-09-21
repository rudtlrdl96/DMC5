#include "Transform.fx"
#include "Light.fx"
#include "RenderBaseValue.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    //float4 NORMAL : NORMAL;
    //float4 TANGENT : TANGENT;
    //float4 BINORMAL : BINORMAL;
    //float4 BLENDWEIGHT : BLENDWEIGHT;
    //int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 TEXCOORD : TEXCOORD;
    float2 ClipUV : TEXCOORD1;
    //float4 VIEWPOSITION : POSITION0;
    //float4 WORLDPOSITION : POSITION1;
    //float4 NORMAL : NORMAL;
    //float4 TANGENT : TANGENT;
    //float4 BINORMAL : BINORMAL;
};

cbuffer EffectVertextData : register(b1)
{
    float IsLockRotation;
    float4 FramePos;
    float4 FrameScale;
    float4 Flip;
};

Output MeshTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
        
    if (0 != IsLockRotation)
    {
        // View 고정
    }
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    
    float4 VtxUV = _Input.TEXCOORD;
    
    if (Flip.x != 0)
    {
        VtxUV.x = 1.0f - VtxUV.x;
    }
    
    if (Flip.y != 0)
    {
        VtxUV.y = 1.0f - VtxUV.y;
    }
        
    NewOutPut.TEXCOORD.x = (VtxUV.x * FrameScale.x) + FramePos.x;
    NewOutPut.TEXCOORD.y = (VtxUV.y * FrameScale.y) + FramePos.y;
    
    NewOutPut.ClipUV = _Input.TEXCOORD.xy;
    
    //NewOutPut.WORLDPOSITION = mul(InputPos, WorldMatrix);
    //NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    
    //_Input.NORMAL.w = 0.0f;
    //NewOutPut.NORMAL = mul(_Input.NORMAL, WorldView);    
    //_Input.TANGENT.w = 0.0f;
    //NewOutPut.TANGENT = mul(_Input.TANGENT, WorldView);    
    //_Input.BINORMAL.w = 0.0f;
    //NewOutPut.BINORMAL = mul(_Input.BINORMAL, WorldView);

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0); // ALBM
//Texture2D NormalTexture : register(t1); // NRMR
//Texture2D SpecularTexture : register(t2); // ATOS

SamplerState ENGINEBASE : register(s0);

struct PixelOutPut
{
    float4 Target : SV_Target0;
};

cbuffer EffectData : register(b2)
{
    float ClipStartX;
    float ClipEndX;    
    float ClipStartY;
    float ClipEndY;    
};

PixelOutPut MeshTexture_PS(Output _Input)
{
    PixelOutPut Result = (PixelOutPut) 0;
    
    
    float2 UV = _Input.TEXCOORD.xy;
    float2 ClipUV = _Input.ClipUV.xy;
    
    if (ClipStartX > ClipUV.x || ClipEndX < ClipUV.x)
    {
        clip(-1);
    }   
    
    if (ClipStartY > ClipUV.y || ClipEndY < ClipUV.y)
    {
        clip(-1);
    }
        
    Result.Target = DiffuseTexture.Sample(ENGINEBASE, UV);
    
    
    // 추후 반투명 처리 해야함
    if (1.0f != Result.Target.a)
    {
        clip(-1);
    }
    
    Result.Target.rgb *= BaseColor.rgb;
    return Result;
}