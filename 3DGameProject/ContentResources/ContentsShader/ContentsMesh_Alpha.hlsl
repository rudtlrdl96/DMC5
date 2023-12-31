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
Texture2D NormalTexture : register(t1); // NRMR
Texture2D SpecularTexture : register(t2); // ATOS

SamplerState ENGINEBASE : register(s0);

struct AlphaOutPut
{
    float4 ResultColor : SV_Target0;
    float4 DistortionColor : SV_Target1;
    float4 MaskTarget : SV_Target2;
};

AlphaOutPut MeshTexture_PS(Output _Input)
{
    AlphaOutPut Result = (AlphaOutPut)0;
        
    // rgb = ����, a = metallicValue 
    float4 AlbmData = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // rgb = NormalMap, a = smoothnessValue 
    float4 NrmrData = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // r = Alpha, gba = sss (subsurface scattering)
    float4 AtosData = SpecularTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    if (0.0f >= AtosData.r)
    {
        clip(-1);
    }
        
    Result.ResultColor.rgb = AlbmData.rgb;
        
    float4 Normal = _Input.NORMAL;
    
    if (0 != IsNormal)
    {
        // WorldView Normal    
        Normal = NormalTexCalculate(NormalTexture, ENGINEBASE, _Input.TEXCOORD, _Input.TANGENT, _Input.BINORMAL, _Input.NORMAL);
    }
    
    float roughness = NrmrData.r;
    float metallic = saturate(AlbmData.a);
    
    
    float3 DiffuseRatio = (float3) 0.0f;
    float3 SpacularRatio = (float3) 0.0f;
    float3 AmbientRatio = (float3) 0.0f;
    
    ResultLight CalLightData = CalLight(0, _Input.VIEWPOSITION, Normal, metallic);
        
    DiffuseRatio += CalLightData.CurLightDiffuseRatio;
    SpacularRatio += CalLightData.CurLightSpacularRatio;
    AmbientRatio += CalLightData.CurLightAmbientRatio;
    
    Result.ResultColor.rgb = Result.ResultColor.rgb * (DiffuseRatio + SpacularRatio + AmbientRatio);
    Result.ResultColor.a = AtosData.r;
    Result.ResultColor += AddColor;
    Result.ResultColor *= MulColor;
    
    Result.DistortionColor = (float4) 0;
    
    if (0 != IsBlurMask)
    {
        Result.MaskTarget = Result.ResultColor;
    }
    else
    {
        Result.MaskTarget = float4(0, 0, 0, 0);
    }
    
    return Result;
}