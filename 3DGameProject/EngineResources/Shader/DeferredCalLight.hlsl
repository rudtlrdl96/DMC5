#include "Light.fx"
#include "RenderBaseValue.fx"

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
    Output NewOutPut = (Output) 0;
    NewOutPut.POSITION = _Input.POSITION;
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    return NewOutPut;
}

struct LightOutPut
{
    float4 DifLight : SV_Target0;
    float4 SpcLight : SV_Target1;
    float4 AmbLight : SV_Target2;
    float4 BackLight : SV_Target3;
};

Texture2D PositionTex : register(t0); //rgb = pos
Texture2D NormalTex : register(t1); // rgb = normal
Texture2D MatTex : register(t2); // r = metal, g = roughness
Texture2D GleamTex : register(t3); // rgb = Gleam light
Texture2D ShadowTex : register(t4);
TextureCube PointShadowTex : register(t5); // Box ShadowMap

//Texture2D SSSTex : register(t5); // Subsurface scattering

SamplerState ENGINEBASE : register(s0);

float GGX_Distribution(float3 normal, float3 halfVector, float roughness)
{
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float roughnessSqr = roughness * roughness;
    float a = roughnessSqr * roughnessSqr;
    float denominator = (NdotH * NdotH * (a - 1.0f) + 1.0f);
    return a / (3.14f * denominator * denominator);
}

//float3 CalBackLight(float4 _SSSData, float3 _Ambient, float3 _Pos, int _LightIndex, float4 _Normal)
//{
//    float3 Result = (float3)0;
//        
//    _Normal.xyz = normalize(_Normal.xyz); // N
//    
//    float4 CalLightDir = (float4)0;
//    
//    if (0 == AllLight[_LightIndex].LightType)
//    {
//        CalLightDir.xyz = normalize(AllLight[_LightIndex].ViewLightDir.xyz); // L
//    }
//    else
//    {
//        CalLightDir.xyz = normalize(_Pos.xyz - AllLight[_LightIndex].ViewLightPos.xyz); // L
//    }
//    
//    float3 CameraView = _Pos - AllLight[0].CameraPosition.xyz;
//    
//    float3 H = normalize(CalLightDir + _Normal * _SSSData.b);
//    float vDotH = pow(saturate(dot(CameraView.xyz, -H)), 1.0f + 0.000001f) * 1.0f;
//    Result = _SSSData.r * (vDotH + _Ambient) * _SSSData.g * AllLight[_LightIndex].LightColor.xyz;
//    
//    return Result;
//}

#define SAMPLES_COUNT 32 
#define INV_SAMPLES_COUNT (1.0f / SAMPLES_COUNT)

float2 VogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
    float GoldenAngle = 2.4f;

    float r = sqrt(sampleIndex + 0.5f) / sqrt(samplesCount);
    float theta = sampleIndex * GoldenAngle + phi;

    float sine, cosine;
    sincos(theta, sine, cosine);
      
    return float2(r * cosine, r * sine);
}

float CalShadow(float4 _WorldPos, int _LightType)
{
    _WorldPos.a = 1.0f;
                
    float4 ShadowWorldPos = mul(_WorldPos, AllLight[LightCount].CameraViewInverseMatrix);
    float4 ShadowWorldViewProjectionPos = mul(ShadowWorldPos, AllLight[LightCount].LightViewProjectionMatrix);
    float3 ShadowWorldProjection = ShadowWorldViewProjectionPos.xyz / ShadowWorldViewProjectionPos.w;
                    
    if (0 == AllLight[LightCount].LightType) // Dir
    {
        float2 ShadowUV = float2(ShadowWorldProjection.x * 0.5f + 0.5f, ShadowWorldProjection.y * -0.5f + 0.5f);
        float ShadowValue = 0.0f;
        
        for (int i = 0; i < SAMPLES_COUNT; ++i)
        {
            float2 VegelPos = VogelDiskSample(i, SAMPLES_COUNT, 1.0f);
            
            VegelPos.x /= (AllLight[LightCount].ShadowTargetSizeX * 0.5f);
            VegelPos.y /= (AllLight[LightCount].ShadowTargetSizeY * 0.5f);
            
            float2 CalPos = ShadowUV + VegelPos;
            float ShadowDepthValue = ShadowTex.Sample(ENGINEBASE, CalPos.xy).r;
            
            if (0.001f < ShadowUV.x && 0.999f > ShadowUV.x &&
            0.001f < ShadowUV.y && 0.999f > ShadowUV.y &&
            ShadowWorldProjection.z >= (ShadowDepthValue + 0.001f))
            {
                ShadowValue += INV_SAMPLES_COUNT;
            }
        }
                
        return max(0.01f, 1.0f - ShadowValue);
    }
    else if (1 == AllLight[LightCount].LightType) // Point
    {
        float4 ShadowLightPos = AllLight[LightCount].LightPos;
        float3 LightUV = (ShadowWorldPos.xyz - ShadowLightPos.xyz);
        float CurDepthValue = length(LightUV);
        float ShadowValue = 0.0f;
        
        for (int i = 0; i < SAMPLES_COUNT; ++i)
        {
            float3 VegelPos = VogelDiskSample(i, SAMPLES_COUNT, 1.0f).xyy;
            VegelPos.xyz /= (AllLight[LightCount].ShadowTargetSizeX * 0.5f);
            
            float3 CalPos = normalize(LightUV) + VegelPos;
            
            float ShadowDepthValue = PointShadowTex.Sample(ENGINEBASE, normalize(CalPos)).r;
        
            if (CurDepthValue >= (ShadowDepthValue + 5.0f))
            {
                ShadowValue += INV_SAMPLES_COUNT;
            }        
        }
                
        return max(0.01f, 1.0f - ShadowValue);
    }
    else if (2 == AllLight[LightCount].LightType) // Spot
    {
        float4 ShadowLightPos = AllLight[LightCount].LightPos;
        float2 ShadowUV = float2(ShadowWorldProjection.x * 0.5f + 0.5f, ShadowWorldProjection.y * -0.5f + 0.5f);
        float CurDepthValue = length(ShadowWorldPos.xyz - ShadowLightPos.xyz);
        float ShadowValue = 0.0f;
        
        for (int i = 0; i < SAMPLES_COUNT; ++i)
        {
            float2 VegelPos = VogelDiskSample(i, SAMPLES_COUNT, 1.0f);
            
            VegelPos.x /= (AllLight[LightCount].ShadowTargetSizeX * 0.5f);
            VegelPos.y /= (AllLight[LightCount].ShadowTargetSizeY * 0.5f);
            
            float2 CalPos = ShadowUV + VegelPos;
            float ShadowDepthValue = ShadowTex.Sample(ENGINEBASE, CalPos.xy).r;
            
            if (CurDepthValue >= (ShadowDepthValue + 5.0f))
            {
                ShadowValue += INV_SAMPLES_COUNT;
            }
        }        
        
        return max(0.01f, 1.0f - ShadowValue);
    }
        
    return 1.0f;
}

LightOutPut DeferredCalLight_PS(Output _Input)
{
    LightOutPut NewOutPut = (LightOutPut) 0;
    
    float4 DeferredPosition = PositionTex.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 Normal = NormalTex.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 Mat = MatTex.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 Gleam = GleamTex.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    //float4 SSSData = SSSTex.Sample(POINTSAMPLER, _Input.TEXCOORD.xy);
    
    float4 DiffuseRatio = (float4) 0.0f;
    float4 SpacularRatio = (float4) 0.0f;
    float4 AmbientRatio = (float4) 0.0f;
    //float4 BackRatio = (float4) 0.0f;
      
    if (0 == DeferredPosition.z)
    {
        clip(-1);
    }
    
    ResultLight CalLightValue = CalLight(LightCount, DeferredPosition, Normal, Mat.r);
    float ShadowValue = CalShadow(DeferredPosition, AllLight[LightCount].LightType);
        
    CalLightValue.CurLightDiffuseRatio *= ShadowValue;
    CalLightValue.CurLightSpacularRatio *= ShadowValue;
    
    DiffuseRatio.xyz += CalLightValue.CurLightDiffuseRatio.xyz;
    SpacularRatio.xyz += CalLightValue.CurLightSpacularRatio.xyz;
    AmbientRatio.xyz += CalLightValue.CurLightAmbientRatio.xyz;
    
    //BackRatio.xyz = CalBackLight(SSSData, AmbientRatio.xyz, DeferredPosition.xyz, LightCount, Normal);
    
    if (0 == LightCount)
    {
        SpacularRatio += float4(Gleam.r, Gleam.g, Gleam.b, 0);
    }
        
    DiffuseRatio.a = 1.0f;
    SpacularRatio.a = 1.0f;
    AmbientRatio.a = 1.0f;
    //BackRatio.a = 1.0f;
    
    NewOutPut.DifLight = DiffuseRatio;
    NewOutPut.SpcLight = SpacularRatio;
    NewOutPut.AmbLight = AmbientRatio;
    //NewOutPut.BackLight = BackRatio;
    
    return NewOutPut;
}
