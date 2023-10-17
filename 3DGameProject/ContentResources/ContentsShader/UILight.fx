struct LightData
{
    float4x4 LightViewMatrix;
    float4x4 LightViewInverseMatrix;
    float4x4 LightProjectionMatrix;
    float4x4 LightProjectionInverseMatrix;
    float4x4 LightViewProjectionMatrix;
    float4x4 CameraView;
    float4x4 CameraViewInverseMatrix;
    float4 LightColor;
    float4 AmbientLight;
    float4 LightPos;
    float4 LightDir;
    float4 LightRevDir;
    float4 ViewLightPos;
    float4 ViewLightDir;
    float4 ViewLightRevDir;
    float4 CameraPosition;
    float4 CameraWorldPosition;
    float ShadowTargetSizeX;
    float ShadowTargetSizeY;
    float LightNear;
    float LightFar;
    float DifLightPower;
    float SpcLightPower;
    float AmbLightPower;
    float SpcPow;
    int LightType;
    float LightRange;
    float LightAngle;
    float LightPower;
    int Temp2;
    int Temp3;
};

#define PIE 3.141592653589793238462643383279502884197169399375105820974944f
#define PIE2 (PIE * 2.0f)
#define DegToRad (PIE / 180)
#define RadToDeg (180 / PIE)

float4 CalDiffuseLight(float4 _Pos, float4 _Normal, LightData _Data)
{
    float4 ResultRatio = (float4) 0.0f;

    _Normal.xyz = normalize(_Normal.xyz); // N
    
    float4 LightRevDir = (float4) 0;
    
    if (0 == _Data.LightType)
    {
        LightRevDir.xyz = normalize(_Data.ViewLightRevDir.xyz); // L
    }
    else
    {
        LightRevDir.xyz = normalize(_Data.ViewLightPos.xyz - _Pos.xyz); // L
    }
    
    
    ResultRatio = max(0.0f, dot(_Normal.xyz, LightRevDir.xyz));
    return ResultRatio;
}

float4 CalSpacularLight(float4 _Pos, float4 _Normal, LightData _Data)
{
    float4 SpacularLight = (float4) 0.0f;
    
    _Normal.xyz = normalize(_Normal.xyz); // N
    
    float4 LightRevDir = (float4) 0;
    
    if (0 == _Data.LightType)
    {
        LightRevDir.xyz = normalize(_Data.ViewLightRevDir.xyz); // L
    }
    else
    {
        LightRevDir.xyz = normalize(_Data.ViewLightPos.xyz - _Pos.xyz); // L
    }
    
    // 반사벡터
    float3 Reflection = normalize(2.0f * _Normal.xyz * dot(LightRevDir.xyz, _Normal.xyz) - LightRevDir.xyz); //  N
    
    // 눈이 어디있냐
    float3 Eye = normalize(_Data.CameraPosition.xyz - _Pos.xyz); // L
    
    // 0 ~ 1
    float Result = max(0.0f, dot(Reflection.xyz, Eye.xyz));
    
    // SpacularLight.xyzw = Result;
    SpacularLight.xyzw = pow(Result, 10);
    
    return SpacularLight;

}

float3 CalReflection(float3 _Normal, float3 _View)
{
    float3 Result = (float3) 0;
       
    Result = normalize(2.0f * _Normal.xyz * dot(_View.xyz, _Normal.xyz) - _View.xyz);
    
    return Result;
}

float4 CalAmbientLight(LightData _LightData)
{
    return _LightData.AmbientLight;
}

float4 NormalTexCalculate(Texture2D NormalTex, SamplerState Smp, float4 UV, float4 _Tangent, float4 _BiNormal, float4 _Normal)
{
    float4 TexNormal = NormalTex.Sample(Smp, UV.xy);
    
    // 색상에는 음수색상이라는게 없어서
    // 노말을 계산한걸 텍스처로 만들때
    // +1 * 0.5f를 한다면 을 해버린다.
    // 그걸 원래대로 돌려주는 공식
    TexNormal = TexNormal * 2.0f - 1.0f;
    TexNormal.w = 0.0f;
    TexNormal = normalize(TexNormal);
    // 그냥 텍스처에서 가져온 normal 아직 회전안되었다.
    
    
    float4 Result = (float4) 0.0f;
    
    // 새로운 회전행렬을 만들어 낸다.
    // 탄젠트 스페이스라고 부른다.
    float3x3 TangentSpace = float3x3(_Tangent.xyz, _BiNormal.xyz, _Normal.xyz);
    Result.xyz = mul(TexNormal.xyz, TangentSpace);
    
    return Result;
}

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

ResultLight CalLight(LightData _LightData, float4 _Position, float4 _Normal, float _Metal)
{
    ResultLight Result;
    
    Result.CurLightDiffuseRatio = float3(0, 0, 0);
    Result.CurLightSpacularRatio = float3(0, 0, 0);
    Result.CurLightAmbientRatio = float3(0, 0, 0);
    
    float LightPower = _LightData.LightPower;
        
    if (0 != _LightData.LightType)
    {
        float Distance = length(_LightData.ViewLightPos.xyz - _Position.xyz);
            
        float FallOffStart = _LightData.LightRange * 0.2f;
        float FallOffEnd = _LightData.LightRange;
                        
        LightPower *= saturate((FallOffEnd - Distance) / (FallOffEnd - FallOffStart));
    }
        
    if (2 == _LightData.LightType)
    {
        // ToLight
        float3 LightVec = normalize(_Position.xyz - _LightData.ViewLightPos.xyz);
        float3 SpotDirToLight = normalize(_LightData.ViewLightDir.xyz);
       
        float CosAng = acos(dot(SpotDirToLight, LightVec)) * RadToDeg;
        float LightAng = _LightData.LightAngle * 0.5f;
        
        float ConAtt = saturate((LightAng - CosAng) / LightAng);
        
        LightPower *= (ConAtt * ConAtt);
    }
        
    if (0.0f < LightPower)
    {
        Result.CurLightDiffuseRatio = CalDiffuseLight(_Position, _Normal, _LightData).xyz;
        Result.CurLightSpacularRatio = CalSpacularLight(_Position, _Normal, _LightData).xyz * (1.0f - _Metal) * 0.5f;
        Result.CurLightAmbientRatio = CalAmbientLight(_LightData).xyz;
        
        Result.CurLightDiffuseRatio *= _LightData.LightColor.xyz * LightPower * _LightData.DifLightPower;
        Result.CurLightSpacularRatio *= _LightData.LightColor.xyz * LightPower * _LightData.SpcLightPower;
        Result.CurLightAmbientRatio *= _LightData.LightColor.xyz * LightPower * _LightData.AmbLightPower;
    }
    
    return Result;
}

cbuffer UILightDatas : register(b12)
{
    LightData UILightData;
}

half3 GetOffSpecularPeakReflectionDir(half3 Normal, half3 ReflectionVector, half Roughness)
{
    half a = Roughness * Roughness;
    
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