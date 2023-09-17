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
TextureCube ReflectionTexture : register(t3); // Reflection Cubemap

SamplerState ENGINEBASE : register(s0);
SamplerState CUBEMAPSAMPLER : register(s1);


struct DeferredOutPut
{
    float4 DifTarget : SV_Target1;
    float4 PosTarget : SV_Target2;
    float4 NorTarget : SV_Target3;
    float4 MatTarget : SV_Target4;
    float4 GlamTarget : SV_Target5;
};

float GGX_Distribution(float3 normal, float3 halfVector, float roughness)
{
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float roughnessSqr = roughness * roughness;
    float a = roughnessSqr * roughnessSqr;
    float denominator = (NdotH * NdotH * (a - 1.0f) + 1.0f);
    return a / (3.14f * denominator * denominator);
}

DeferredOutPut MeshTexture_PS(Output _Input)
{
    DeferredOutPut Result = (DeferredOutPut)0;
    
    // rgb = 색상, a = metallicValue 
    float4 AlbmData = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // rgb = NormalMap, a = smoothnessValue 
    float4 NrmrData = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // r = Alpha, gba = sss (subsurface scattering)
    float4 AtosData = SpecularTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
                   
    if (0.0f >= AtosData.r)
    {
        clip(-1);
    }
    
    Result.PosTarget = _Input.VIEWPOSITION;    
    Result.DifTarget = float4(AlbmData.r, AlbmData.g, AlbmData.b, AtosData.r);
            
    if (0 != IsNormal)
    {
        // WorldView Normal    
        Result.NorTarget = NormalTexCalculate(NormalTexture, ENGINEBASE, _Input.TEXCOORD, _Input.TANGENT, _Input.BINORMAL, _Input.NORMAL);
    }
    else
    {
        Result.NorTarget = _Input.NORMAL;
    }

            
    // 반사량 계산 공식 러프니스 값에 따라서 결정된다        
    float roughness = 1.0 - NrmrData.r; // smoothness는 러프니스 값입니다.
    float3 reflection = reflect(AllLight[0].LightRevDir.xyz, Result.NorTarget.xyz); // 빛의 반사 방향 계산
    float distribution = GGX_Distribution(Result.NorTarget.xyz, reflection, roughness); // 반사 분포 계산
                               
    // Eye        
    
    float3 CameraPos = AllLight[0].CameraPosition;
    float3 RefViewPos = _Input.VIEWPOSITION.xyz;
    float3 CameraView = normalize(CameraPos - RefViewPos);    
        
    // 반사벡터
    float3 refnormal = Result.NorTarget.xyz;
    float3 refvector = normalize(2.0f * refnormal.xyz * dot(CameraView.xyz, refnormal.xyz) - CameraView.xyz);
    refvector.y = -refvector.y;    
    
    float4 ReflectionColor = ReflectionTexture.Sample(CUBEMAPSAMPLER, -refvector);
    
    // 계산된 메탈릭 값
    float metallic = saturate(AlbmData.a - distribution);
     
    // AlbmData -> metallicValue 값에 따라서 결정되어야 한다        
    Result.DifTarget.rgb = lerp(AlbmData.rgb, float3(0, 0, 0), metallic);
    Result.DifTarget.rgb += lerp(float3(0, 0, 0), ReflectionColor.rgb * 0.5f, metallic);
    
    Result.DifTarget.a = 1.0f;
    Result.PosTarget.a = 1.0f;
    Result.NorTarget.a = 1.0f;    
    
    Result.MatTarget.r = metallic;
    Result.MatTarget.g = roughness;
    Result.MatTarget.a = 1.0f;
    
    if(0 != BaseColor.r)
    {
        Result.GlamTarget.rgb = AlbmData.rgb * BaseColor.r;
    }
    
    Result.GlamTarget.a = 1.0f;
        
    return Result;
}