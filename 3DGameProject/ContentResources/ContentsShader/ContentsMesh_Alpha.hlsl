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
};

float GGX_Distribution(float3 normal, float3 halfVector, float roughness)
{
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float roughnessSqr = roughness * roughness;
    float a = roughnessSqr * roughnessSqr;
    float denominator = (NdotH * NdotH * (a - 1.0f) + 1.0f);
    return a / (3.14f * denominator * denominator);
}

AlphaOutPut MeshTexture_PS(Output _Input)
{
    AlphaOutPut Result = (AlphaOutPut)0;
    
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
        
    Result.ResultColor.rgb = AlbmData.rgb;
        
    float4 Normal = _Input.NORMAL;
    
    if (0 != IsNormal)
    {
        // WorldView Normal    
        Normal = NormalTexCalculate(NormalTexture, ENGINEBASE, _Input.TEXCOORD, _Input.TANGENT, _Input.BINORMAL, _Input.NORMAL);
    }
    
    // 반사량 계산 공식 러프니스 값에 따라서 결정된다        
    float roughness = 1.0 - NrmrData.r; // smoothness는 러프니스 값입니다.
    float3 reflection = reflect(AllLight[0].LightRevDir.xyz, Normal.xyz); // 빛의 반사 방향 계산
    float distribution = GGX_Distribution(Normal.xyz, reflection, roughness); // 반사 분포 계산
                               
    // 계산된 메탈릭 값
    float metallic = saturate(AlbmData.a - distribution);
     
    // AlbmData -> metallicValue 값에 따라서 결정되어야 한다        
    //Result.ResultColor.rgb = lerp(AlbmData.rgb, float3(0, 0, 0), metallic);
    
    float4 DiffuseRatio = (float4) 0.0f;
    float4 SpacularRatio = (float4) 0.0f;
    float4 AmbientRatio = (float4) 0.0f;
    
    for (int i = 0; i < LightCount; ++i)
    {
        float LightPower = AllLight[i].LightPower;
        
        if (0 != AllLight[i].LightType)
        {
            float Distance = length(AllLight[i].ViewLightPos.xyz - _Input.VIEWPOSITION.xyz);
            
            // 200
            float FallOffStart = AllLight[i].LightRange * 0.2f;
            
            // 1000
            float FallOffEnd = AllLight[i].LightRange;
            
            if (Distance > FallOffEnd)
            {
                continue;
            }
            
            LightPower *= saturate((FallOffEnd - Distance) / (FallOffEnd - FallOffStart));
        }
        
        if (2 == AllLight[i].LightType)
        {
            float3 LightVec = normalize(AllLight[i].ViewLightPos.xyz - _Input.VIEWPOSITION.xyz);
            float3 SpotCone = pow(saturate(dot(LightVec, normalize(AllLight[i].ViewLightDir.xyz))), AllLight[i].LightAngle);
            
            LightPower *= SpotCone;
        }
        
        if (0.0f < LightPower)
        {
            // Diffuse Light 계산
            DiffuseRatio.xyz += AllLight[i].LightColor.xyz * CalDiffuseLight(_Input.VIEWPOSITION, Normal, AllLight[i]).xyz * LightPower;
        
            // Spacular Light 계산
            SpacularRatio.xyz += AllLight[i].LightColor.xyz * CalSpacularLight(_Input.VIEWPOSITION, Normal, AllLight[i]).xyz * (1.0f - metallic) * LightPower;
        }
    }
    
    AmbientRatio = AllLight[0].AmbientLight;
    
    Result.ResultColor.rgb = Result.ResultColor.rgb * (DiffuseRatio.rgb + SpacularRatio.rgb + AmbientRatio.rgb);
    Result.ResultColor.a = AtosData.r;
      
    Result.ResultColor *= BaseColor;
    
    return Result;
}