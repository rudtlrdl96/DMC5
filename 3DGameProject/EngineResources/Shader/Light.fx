
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
    float4 CameraView;
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
    _Data.ViewLightRevDir.xyz = normalize(_Data.ViewLightRevDir.xyz); // L
    
    float4 LightRevDir = (float4) 0;
    
    if (0 == _Data.LightType)
    {
        LightRevDir.xyz = normalize(_Data.ViewLightRevDir.xyz); // L
    }
    else
    {
        LightRevDir.xyz = normalize(_Data.ViewLightPos.xyz - _Pos.xyz); // L
    }
    
    // �ݻ纤��
    // 
    float3 Reflection = normalize(2.0f * _Normal.xyz * dot(LightRevDir.xyz, _Normal.xyz) - LightRevDir.xyz); //  N
    
    // ���� ����ֳ�
    float3 Eye = normalize(_Data.CameraPosition.xyz - _Pos.xyz); // L
    
    // 0 ~ 1
    float Result = max(0.0f, dot(Reflection.xyz, Eye.xyz));
    
    // SpacularLight.xyzw = Result;
    SpacularLight.xyzw = pow(Result, 10);
    
    return SpacularLight;

}

float4 CalAmbientLight(LightData _LightData)
{
    return _LightData.AmbientLight;
}

float4 NormalTexCalculate(float4 _NormalValue, float4 UV, float4 _Tangent, float4 _BiNormal, float4 _Normal)
{
    // ���󿡴� ���������̶�°� ���
    // �븻�� ����Ѱ� �ؽ�ó�� ���鶧
    // +1 * 0.5f�� �Ѵٸ� �� �ع�����.
    // �װ� ������� �����ִ� ����
    _NormalValue = _NormalValue * 2.0f - 1.0f;
    _NormalValue.w = 0.0f;
    _NormalValue = normalize(_NormalValue);
    // �׳� �ؽ�ó���� ������ normal ���� ȸ���ȵǾ���.
    
    
    float4 Result = (float4) 0.0f;
    
    // ���ο� ȸ������� ����� ����.
    // ź��Ʈ �����̽���� �θ���.
    float3x3 TangentSpace = float3x3(_Tangent.xyz, _BiNormal.xyz, _Normal.xyz);
    Result.xyz = mul(_NormalValue.xyz, TangentSpace);
    
    return Result;

}


