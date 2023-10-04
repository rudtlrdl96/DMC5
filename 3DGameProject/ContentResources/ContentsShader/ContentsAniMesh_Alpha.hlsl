#include "Transform.fx"
#include "Light.fx"
#include "RenderBaseValue.fx"
#include "Animation.fx"

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

Output MeshAniTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
    }
    
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
        Result.CurLightDiffuseRatio = AllLight[_LightIndex].LightColor.xyz * CalDiffuseLight(_Position, _Normal, AllLight[_LightIndex]).xyz * LightPower;
        Result.CurLightSpacularRatio = AllLight[_LightIndex].LightColor.xyz * CalSpacularLight(_Position, _Normal, AllLight[_LightIndex]).xyz * (1.0f - _Metal) * LightPower * 0.5f;
        Result.CurLightAmbientRatio = AllLight[_LightIndex].LightColor.xyz * CalAmbientLight(AllLight[_LightIndex]).xyz * LightPower;
    }
    
    return Result;
}

AlphaOutPut MeshTexture_PS(Output _Input)
{
    AlphaOutPut Result = (AlphaOutPut) 0;
    
    // rgb = ����, a = metallicValue 
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
    
    // �ݻ緮 ��� ���� �����Ͻ� ���� ���� �����ȴ�        
    float roughness = 1.0 - NrmrData.r; // smoothness�� �����Ͻ� ���Դϴ�.
    float3 reflection = reflect(AllLight[0].LightRevDir.xyz, Normal.xyz); // ���� �ݻ� ���� ���
    float distribution = GGX_Distribution(Normal.xyz, reflection, roughness); // �ݻ� ���� ���
                               
    // ���� ��Ż�� ��
    float metallic = saturate(AlbmData.a - distribution);
     
    // AlbmData -> metallicValue ���� ���� �����Ǿ�� �Ѵ�        
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
    
    Result.ResultColor.rgb = Result.ResultColor.rgb * (DiffuseRatio.rgb + SpacularRatio.rgb + AmbientRatio.rgb);
    Result.ResultColor.a = AtosData.r;
    Result.ResultColor += AddColor;
    Result.ResultColor *= MulColor;
    
    return Result;
}