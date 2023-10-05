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


struct ResultLight
{
    float3 CurLightDiffuseRatio;
    float3 CurLightSpacularRatio;
    float3 CurLightAmbientRatio;
};

ResultLight CalLight(int _LightIndex, float4 _Position, float4 _Normal, float _Metal)
{
    ResultLight Result;
    
    Result.CurLightDiffuseRatio = float3(0, 0, 0);
    Result.CurLightSpacularRatio = float3(0, 0, 0);
    Result.CurLightAmbientRatio = float3(0, 0, 0);
    
    float LightPower = AllLight[_LightIndex].LightPower;
        
    if (0 != AllLight[_LightIndex].LightType)
    {
        float Distance = length(AllLight[_LightIndex].ViewLightPos.xyz - _Position.xyz);
            
        float FallOffStart = AllLight[_LightIndex].LightRange * 0.2f;
        float FallOffEnd = AllLight[_LightIndex].LightRange;
                        
        LightPower *= saturate((FallOffEnd - Distance) / (FallOffEnd - FallOffStart));
    }
        
    if (2 == AllLight[_LightIndex].LightType)
    {
        // ToLight
        float3 LightVec = normalize(_Position.xyz - AllLight[_LightIndex].ViewLightPos.xyz);
        float3 SpotDirToLight = normalize(AllLight[_LightIndex].ViewLightDir.xyz);
       
        float CosAng = acos(dot(SpotDirToLight, LightVec)) * RadToDeg;
        float LightAng = AllLight[_LightIndex].LightAngle * 0.5f;
        
        float ConAtt = saturate((LightAng - CosAng) / LightAng);
        
        LightPower *= (ConAtt * ConAtt);
    }
        
    if (0.0f < LightPower)
    {
        Result.CurLightDiffuseRatio = CalDiffuseLight(_Position, _Normal, AllLight[_LightIndex]).xyz;
        Result.CurLightSpacularRatio = CalSpacularLight(_Position, _Normal, AllLight[_LightIndex]).xyz * (1.0f - _Metal) * 0.5f;
        Result.CurLightAmbientRatio = CalAmbientLight(AllLight[_LightIndex]).xyz;
        
        Result.CurLightDiffuseRatio *= AllLight[_LightIndex].LightColor.xyz * LightPower * AllLight[_LightIndex].DifLightPower;
        Result.CurLightSpacularRatio *= AllLight[_LightIndex].LightColor.xyz * LightPower * AllLight[_LightIndex].SpcLightPower;
        Result.CurLightAmbientRatio *= AllLight[_LightIndex].LightColor.xyz * LightPower * AllLight[_LightIndex].AmbLightPower;
    }
    
    return Result;
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
    
    
    float3 DiffuseRatio = (float3) 0.0f;
    float3 SpacularRatio = (float3) 0.0f;
    float3 AmbientRatio = (float3) 0.0f;
    
    for (int i = 0; i < LightCount; ++i)
    {
        ResultLight CalLightData = CalLight(i, _Input.VIEWPOSITION, _Input.NORMAL, metallic);
        
        DiffuseRatio += CalLightData.CurLightDiffuseRatio;
        SpacularRatio += CalLightData.CurLightSpacularRatio;
        AmbientRatio += CalLightData.CurLightAmbientRatio;
    }
    
    Result.ResultColor.rgb = Result.ResultColor.rgb * (DiffuseRatio + SpacularRatio + AmbientRatio);
    Result.ResultColor.a = AtosData.r;
    Result.ResultColor += AddColor;
    Result.ResultColor *= MulColor;
    
    return Result;
}