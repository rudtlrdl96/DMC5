#include "Light.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct Output
{
    float4 POSITION : SV_Position;
    float4 TEXCOORD : TEXCOORD;
};

Output DeferredCalLight_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.POSITION = _Input.POSITION;
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    return NewOutPut;
}

struct LightOutPut
{
    float4 DifLight : SV_Target0;
    float4 SpcLight : SV_Target1;
    float4 AmbLight : SV_Target2;
};

Texture2D PositionTex : register(t0); //rgb = pos
Texture2D NormalTex : register(t1); // rgb = normal
Texture2D MatTex : register(t2); // r = metal, g = roughness
Texture2D GleamTex : register(t3); // rgb = Gleam light
Texture2DArray ShadowTex : register(t4);

SamplerState POINTSAMPLER : register(s0);

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
};

ResultLight CalLight(int _LightIndex, float4 _Position, float4 _Normal, float _Metal)
{
    ResultLight Result;
    
    Result.CurLightDiffuseRatio = float3(0, 0, 0);
    Result.CurLightSpacularRatio = float3(0, 0, 0);
    
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
        float3 LightVec = normalize(AllLight[_LightIndex].ViewLightPos.xyz - _Position.xyz);
        float SpotCone = pow(saturate(dot(LightVec, normalize(AllLight[_LightIndex].ViewLightDir.xyz))), AllLight[_LightIndex].LightAngle);
            
        LightPower *= SpotCone;
    }
        
    if (0.0f < LightPower)
    {
        Result.CurLightDiffuseRatio = AllLight[_LightIndex].LightColor.xyz * CalDiffuseLight(_Position, _Normal, AllLight[_LightIndex]).xyz * LightPower;
        Result.CurLightSpacularRatio = AllLight[_LightIndex].LightColor.xyz * CalSpacularLight(_Position, _Normal, AllLight[_LightIndex]).xyz * (1.0f - _Metal) * LightPower * 0.5f;
    }
    
    return Result;
}


LightOutPut DeferredCalLight_PS(Output _Input)
{
    LightOutPut NewOutPut = (LightOutPut) 0;
    
    float4 DeferredPosition = PositionTex.Sample(POINTSAMPLER, _Input.TEXCOORD.xy);
    float4 Normal = NormalTex.Sample(POINTSAMPLER, _Input.TEXCOORD.xy);
    float4 Mat = MatTex.Sample(POINTSAMPLER, _Input.TEXCOORD.xy);
    float4 Gleam = GleamTex.Sample(POINTSAMPLER, _Input.TEXCOORD.xy);
            
    if (0 == DeferredPosition.z)
    {
        clip(-1);
    }
    
    float4 DiffuseRatio = (float4) 0.0f;
    float4 SpacularRatio = (float4) 0.0f;
    float4 AmbientRatio = (float4) 0.0f;
      
    for (int i = 0; i < LightCount; ++i)
    {
        ResultLight CalLightValue = CalLight(0, DeferredPosition, Normal, Mat.r);
                        
        float4 ShadowWorldViewPos = DeferredPosition;
        ShadowWorldViewPos.a = 1.0f;
            
        if(0 == i)
        {
            float4 ShadowLightWorldPos = mul(ShadowWorldViewPos, AllLight[i].CameraViewInverseMatrix);
            float4 ShadowLightPos = mul(ShadowLightWorldPos, AllLight[i].LightViewProjectionMatrix);
            float3 ShadowLightProjection = ShadowLightPos.xyz / ShadowLightPos.w;
                
            float2 ShadowUV = float2(ShadowLightProjection.x * 0.5f + 0.5f, ShadowLightProjection.y * -0.5f + 0.5f);
            float ShadowDepthValue = ShadowTex.Sample(POINTSAMPLER, float3(ShadowUV.xy, i)).r;
                        
            if (0.001f < ShadowUV.x && 0.999f > ShadowUV.x &&
            0.001f < ShadowUV.y && 0.999f > ShadowUV.y &&
            ShadowLightProjection.z >= (ShadowDepthValue + 0.001f))
            {
                CalLightValue.CurLightDiffuseRatio *= 0.01f;
                CalLightValue.CurLightSpacularRatio *= 0.01f;
            }        
        }

        DiffuseRatio.xyz += CalLightValue.CurLightDiffuseRatio.xyz;
        SpacularRatio.xyz += CalLightValue.CurLightSpacularRatio.xyz;
    }
    
    SpacularRatio += float4(Gleam.r, Gleam.g, Gleam.b, 0);    
    AmbientRatio = AllLight[0].AmbientLight;
    
    DiffuseRatio.a = 1.0f;
    SpacularRatio.a = 1.0f;
    AmbientRatio.a = 1.0f;
    
    NewOutPut.DifLight = DiffuseRatio;
    NewOutPut.SpcLight = SpacularRatio;
    NewOutPut.AmbLight = AmbientRatio;
    
    return NewOutPut;
}
