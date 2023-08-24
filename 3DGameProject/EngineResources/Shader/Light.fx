
struct LightData 
{
    float4x4 LightViewMatrix;
    float4x4 LightViewInverseMatrix;
    float4x4 LightProjectionMatrix;
    float4x4 LightProjectionInverseMatrix;
    float4x4 LightViewProjectionMatrix;
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
    float LightTargetSizeX;
    float LightTargetSizeY;
    float LightNear;
    float LightFar;
    float DifLightPower;
    float SpcLightPower;
    float AmbLightPower;
    float SpcPow;
    int LightType;
    float PointLightRange;
    int Temp2;
    int Temp3;
};

cbuffer LightDatas : register(b12)
{
    int LightCount;
    LightData AllLight[64];
};

float4 CalDiffuseLight(float4 _Pos,  float4 _Normal, LightData _Data)
{
    float4 ResultRatio = (float4) 0.0f;

    _Normal = normalize(_Normal); // N
    float4 LightRevDir = normalize(_Data.ViewLightRevDir); // L
    
    ResultRatio = max(0.0f, dot(_Normal.xyz, LightRevDir.xyz));
    return ResultRatio;
}

float4 CalAmbientLight(LightData _LightData)
{
    return _LightData.AmbientLight;
}