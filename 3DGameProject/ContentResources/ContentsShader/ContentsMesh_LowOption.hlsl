#include "Transform.fx"
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
    float4 WORLDPOSITION : POSITION1;
    float4 VIEWPOSITION : POSITION2;
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

struct ForwardOutPut
{
    float4 ColorTarget : SV_Target0;
    float4 PosTarget : SV_Target1;
    float4 MaskTarget : SV_Target2;
};

ForwardOutPut MeshTexture_PS(Output _Input)
{
    ForwardOutPut Result;
    
    float4 AlbmData = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 NrmrData = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 AtosData = SpecularTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    Result.PosTarget = _Input.WORLDPOSITION;
    
    if (1.0f != AtosData.r)
    {
        AtosData.r = 1.0f;
        AtosData.g = 0.0f;
    }
    
    float4 Color = float4(AlbmData.rgb, AtosData.r);
    
    Color += AddColor;
    Color *= MulColor;
        
    Result.ColorTarget = Color + (NrmrData * 0.000001f);
    
    if (0 != IsBlurMask)
    {
        Result.MaskTarget = Result.ColorTarget;
    }
    else
    {
        Result.MaskTarget = float4(0, 0, 0, 0);
    }
    
    return Result;
}