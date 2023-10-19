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
        float4x4 InverseView = AllLight[0].CameraViewInverseMatrix;
                
        InverseView[3][0] = 0;
        InverseView[3][1] = 0;
        InverseView[3][2] = 0;
        InverseView[3][3] = 1;
        InverseView[0][3] = 0;
        InverseView[1][3] = 0;
        InverseView[2][3] = 0;
                
        InputPos = mul(InputPos, InverseView);
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
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D MaskTexture : register(t1); // DistortionTexture

SamplerState ENGINEBASE : register(s0);

struct AlphaOutPut
{
    float4 ResultColor : SV_Target0;
    float4 DistortionColor : SV_Target1;
};

cbuffer EffectData : register(b2)
{
    float ClipStartX;
    float ClipEndX;    
    float ClipStartY;
    float ClipEndY;    
    float UVX;    
    float UVY;    
    float4 EffectMulColor;
    float4 EffectPlusColor;
};

cbuffer DistortionData : register(b3)
{
    float4 IsDistortion;
};

cbuffer HSVColorData : register(b4)
{
    float4 HSVColor;
};

AlphaOutPut MeshTexture_PS(Output _Input)
{
    AlphaOutPut Result = (AlphaOutPut) 0;
        
    float2 UV = _Input.TEXCOORD.xy;
    UV.xy += float2(UVX, UVY);
    float2 ClipUV = _Input.ClipUV.xy;
    ClipUV.xy += float2(UVX, UVY);
    
    if (ClipStartX > ClipUV.x || ClipEndX < ClipUV.x)
    {
        clip(-1);
    }   
    
    if (ClipStartY > ClipUV.y || ClipEndY < ClipUV.y)
    {
        clip(-1);
    }
        
    Result.ResultColor = DiffuseTexture.Sample(ENGINEBASE, UV);
               
    Result.ResultColor *= EffectMulColor;
    Result.ResultColor += EffectPlusColor;
    
    float saturation = HSVColor.r * 2;
    float brightness = HSVColor.g * 2 - 1;
    float contrast = HSVColor.b * 2;
    
    Result.ResultColor.rgb = (Result.ResultColor.rgb - 0.5f) * contrast + 0.5f;
    Result.ResultColor.rgb = Result.ResultColor.rgb + brightness;
    float3 intensity = dot(Result.ResultColor.rgb, float3(0.39, 0.59, 0.11));

    Result.ResultColor.a = clamp(Result.ResultColor.a, 0, 1);
        
    if (0 == IsDistortion.x)
    {
        Result.DistortionColor = (float4) 0;
    }
    else
    {
        Result.DistortionColor = MaskTexture.Sample(ENGINEBASE, UV);
        Result.DistortionColor.a = Result.ResultColor.a;
    }
    
    return Result;
}