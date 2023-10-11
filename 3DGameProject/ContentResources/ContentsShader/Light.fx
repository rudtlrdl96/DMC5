#include "../../EngineResources/Shader/Light.fx"

float GGX_Distribution(float3 normal, float3 halfVector, float roughness)
{
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float roughnessSqr = roughness * roughness;
    float a = roughnessSqr * roughnessSqr;
    float denominator = (NdotH * NdotH * (a - 1.0f) + 1.0f);
    return a / (3.14f * denominator * denominator);
}

half3 GetOffSpecularPeakReflectionDir(half3 Normal, half3 ReflectionVector, half Roughness)
{
    half a = Roughness * Roughness;
    return lerp(Normal, ReflectionVector, (1 - a) * (sqrt(1 - a) + a));
}

float3 NoiseWaterNormal(float3 _Normal)
{
    
    
    
    return float3(0, 1, 0);
}