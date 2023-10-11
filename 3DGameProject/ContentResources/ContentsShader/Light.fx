#include "../../EngineResources/Shader/Light.fx"

float GGX_Distribution(float3 normal, float3 halfVector, float roughness)
{
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float roughnessSqr = roughness * roughness;
    float a = roughnessSqr * roughnessSqr;
    float denominator = (NdotH * NdotH * (a - 1.0f) + 1.0f);
    return a / (3.14f * denominator * denominator);
}

float3 GetOffSpecularPeakReflectionDir(float3 Normal, float3 ReflectionVector, float Roughness)
{
    float a = Roughness * Roughness;
    
    return lerp(ReflectionVector, Normal, (1 - a) * (sqrt(1 - a) + a));
}

float hash(float n)
{
    return frac(sin(n) * 32.5454412211233);
}

float2 hash2(float n)
{
    return frac(sin(float2(n, n + 1.0)) * float2(11.1451239123, 34.349430423));
}
float3 hash3(float n)
{
    return frac(sin(float3(n, n + 1.0, n + 2.0)) * float3(84.54531253, 42.145259123, 23.349041223));
}

float noise(in float2 x, Texture2D _NoiseTex, SamplerState _Sample)
{
    float2 p = floor(x);
    float2 f = frac(x);

    float2 uv = p.xy + f.xy * f.xy * (3.0 - 2.0 * f.xy);

    return -1.0 + 2.0 * _NoiseTex.Sample(_Sample, (uv + 0.5) / 256.0, 0.0).x;
}

float noise(in float3 x, Texture2D _NoiseTex, SamplerState _Sample)
{
    float z = x.z * 64.0;
    float2 offz = float2(0.317, 0.123);
    float2 uv1 = x.xy + offz * floor(z);
    float2 uv2 = uv1 + offz;
    
    return lerp(_NoiseTex.Sample(_Sample, uv1).x, _NoiseTex.Sample(_Sample, uv2).x, frac(z)) - 0.5;
}

static const float2x2 m2 = float2x2(0.80, -0.60, 0.60, 0.80);

static const float3x3 m3 = float3x3(0.00, 0.80, 0.60,
                     -0.80, 0.36, -0.48,
                     -0.60, -0.48, 0.64);

float fbm(float3 p, Texture2D _NoiseTex, SamplerState _Sample)
{
    float f = 0.0;
    f += 0.5000 * noise(p, _NoiseTex, _Sample);
    p = mul(m3, p) * 2.02;
    f += 0.2500 * noise(p, _NoiseTex, _Sample);
    p = mul(m3, p) * 2.03;
    f += 0.1250 * noise(p, _NoiseTex, _Sample);
    p = mul(m3, p) * 2.01;
    f += 0.0625 * noise(p, _NoiseTex, _Sample);
    return f / 0.9375;
}

#define BUMPFACTOR 3.0
#define EPSILON 0.1

float waterHeightMap(float2 pos, Texture2D _NoiseTex, SamplerState _Sample)
{
    float2 posm = 0.02 * mul(pos, m2);
    posm.x += 0.001;
    float f = fbm(float3(posm * 1.9, 0.01), _NoiseTex, _Sample);
    float height = 0.5 + 0.1 * f;
    height += 0.05 * sin(posm.x * 6.0 + 10.0 * f);
	
    return height;
}

float3 NoiseWaterNormal(float2 _coord, Texture2D _NoiseTex, SamplerState _Sample)
{
    float2 dx = float2(EPSILON, 0.);
    float2 dz = float2(0., EPSILON);
					
    float3 Normal = float3(0., -1., 0.);
    
    Normal.x = min(1.0f, BUMPFACTOR * (waterHeightMap(_coord + dx, _NoiseTex, _Sample) - waterHeightMap(_coord - dx, _NoiseTex, _Sample)) / (2. * EPSILON));
    Normal.z = min(1.0f, BUMPFACTOR * (waterHeightMap(_coord + dz, _NoiseTex, _Sample) - waterHeightMap(_coord - dz, _NoiseTex, _Sample)) / (2. * EPSILON));
    
    return normalize(Normal);
}