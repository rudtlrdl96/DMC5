
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

cbuffer LightDatas : register(b12)
{
    int LightCount;
    LightData AllLight[64];
};

#define PIE 3.141592653589793238462643383279502884197169399375105820974944f
#define PIE2 (PIE * 2.0f)
#define DegToRad (PIE / 180)
#define RadToDeg (180 / PIE)



float4 CalDiffuseLight(float4 _Pos, float4 _Normal, LightData _Data)
{
    float4 ResultRatio = (float4) 0.0f;

    _Normal.xyz = normalize(_Normal.xyz); // N
    
    float4 LightRevDir = (float4)0;
    
    if(0 == _Data.LightType)
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
        Result.CurLightSpacularRatio = CalSpacularLight(_Position, _Normal, AllLight[_LightIndex]).xyz * (2.0f - _Metal);
        Result.CurLightAmbientRatio = CalAmbientLight(AllLight[_LightIndex]).xyz;
        
        Result.CurLightDiffuseRatio *= AllLight[_LightIndex].LightColor.xyz * LightPower * AllLight[_LightIndex].DifLightPower;
        Result.CurLightSpacularRatio *= AllLight[_LightIndex].LightColor.xyz * LightPower * AllLight[_LightIndex].SpcLightPower;
        Result.CurLightAmbientRatio *= AllLight[_LightIndex].LightColor.xyz * LightPower * AllLight[_LightIndex].AmbLightPower;
    }
    
    return Result;
}