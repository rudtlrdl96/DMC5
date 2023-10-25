#include "Transform.fx"
#include "RenderBaseValue.fx"
#include "UILight.fx"
#include "Animation.fx"

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
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
    }
    
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
    float4 PosTarget : SV_Target1;
};


cbuffer ColorData : register(b1)
{
    float4 HSVColor;
}

cbuffer ClipData : register(b2)
{
    float ClipStartX;
    float ClipEndX;
    float ClipStartY;
    float ClipEndY;
};

AlphaOutPut MeshTexture_PS(Output _Input)
{
    if (ClipStartX > _Input.TEXCOORD.x || ClipEndX < _Input.TEXCOORD.x)
    {
        clip(-1);
    }
    
    if (ClipStartY > _Input.TEXCOORD.y || ClipEndY < _Input.TEXCOORD.y)
    {
        clip(-1);
    }
    
    AlphaOutPut Result = (AlphaOutPut) 0;
       
    Result.PosTarget = _Input.VIEWPOSITION;
    
    // rgb = 색상, a = metallicValue 
    float4 AlbmData = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // r = Alpha, gba = sss (subsurface scattering)
    float4 AtosData = SpecularTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    AlbmData.a = AtosData.r;
    
    AlbmData += AddColor;
    AlbmData *= MulColor;
    
    float saturation = HSVColor.r * 2;
    float brightness = HSVColor.g * 2 - 1;
    float contrast = HSVColor.b * 2;
    
    AlbmData.rgb = (AlbmData.rgb - 0.5f) * contrast + 0.5f;
    AlbmData.rgb = AlbmData.rgb + brightness;
    float3 intensity = dot(AlbmData.rgb, float3(0.39, 0.59, 0.11));

    AlbmData.rgb = lerp(intensity, AlbmData.rgb, saturation);
    
    // rgb = NormalMap, a = smoothnessValue 
    float4 NrmrData = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    if (0.0f >= AtosData.r)
    {
        clip(-1);
    }
        
    Result.ResultColor = AlbmData;
        
    float4 Normal = _Input.NORMAL;
    
    if (0 != IsNormal)
    {
        // WorldView Normal    
        Normal = NormalTexCalculate(NormalTexture, ENGINEBASE, _Input.TEXCOORD, _Input.TANGENT, _Input.BINORMAL, _Input.NORMAL);
    }
    
    // 반사량 계산 공식 러프니스 값에 따라서 결정된다        
    float roughness = 1.0 - NrmrData.r; // smoothness는 러프니스 값입니다.
    float3 reflection = reflect(UILightData.LightRevDir.xyz, Normal.xyz); // 빛의 반사 방향 계산
    float distribution = GGX_Distribution(Normal.xyz, reflection, roughness); // 반사 분포 계산
                               
    // 계산된 메탈릭 값
    float metallic = saturate(AlbmData.a - distribution);
         
    float3 DiffuseRatio = (float3) 0.0f;
    float3 SpacularRatio = (float3) 0.0f;
    float3 AmbientRatio = (float3) 0.0f;
    
    ResultLight CalLightData = CalLight(UILightData, _Input.VIEWPOSITION, Normal, metallic);
        
    DiffuseRatio += CalLightData.CurLightDiffuseRatio;
    SpacularRatio += CalLightData.CurLightSpacularRatio;
    AmbientRatio += CalLightData.CurLightAmbientRatio;
    
    Result.ResultColor.rgb = Result.ResultColor.rgb * (DiffuseRatio + SpacularRatio + AmbientRatio);
    
    return Result;
}