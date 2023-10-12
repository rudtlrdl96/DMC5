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
    float4x4 WORLDMATRIX : MATRIX;
    float4 WVPPOSITION : POSITION5;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL0;
    float4 WORLDNORMAL : NORMAL1;
    float4 TANGENT : TANGENT0;
    float4 WORLDTANGENT : TANGENT1;
    float4 BINORMAL : BINORMAL0;
    float4 WORLDBINORMAL : BINORMAL1;
};

Output MeshTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
        
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    NewOutPut.WVPPOSITION = NewOutPut.POSITION;
    
    NewOutPut.WORLDPOSITION = mul(InputPos, WorldMatrix);
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    NewOutPut.WORLDMATRIX = WorldMatrix;
    
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(_Input.NORMAL, WorldView);
    NewOutPut.WORLDNORMAL = mul(_Input.NORMAL, WorldMatrix);
    
    _Input.TANGENT.w = 0.0f;
    NewOutPut.TANGENT = mul(_Input.TANGENT, WorldView);
    NewOutPut.WORLDTANGENT = mul(_Input.TANGENT, WorldMatrix);
    
    _Input.BINORMAL.w = 0.0f;
    NewOutPut.BINORMAL = mul(_Input.BINORMAL, WorldView);
    NewOutPut.WORLDBINORMAL = mul(_Input.BINORMAL, WorldMatrix);

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0); // ALBM
Texture2D NormalTexture : register(t1); // NRMR
Texture2D SpecularTexture : register(t2); // ATOS
TextureCube ReflectionTexture : register(t3); // Reflection Cubemap
Texture2D WaterNoiseTexture : register(t4); // WaterNoise

SamplerState ENGINEBASE : register(s0);
SamplerState CUBEMAPSAMPLER : register(s1);

struct DeferredOutPut
{
    float4 DifTarget : SV_Target0;
    float4 PosTarget : SV_Target1;
    float4 NorTarget : SV_Target2;
    float4 MatTarget : SV_Target3;
    float4 GlamTarget : SV_Target4;
    float4 SSSTarget : SV_Target5;
};

// float3 R = 2 * dot(V, N) * N - V;
// float NoV = saturate(dot(N, V));

// Point lobe in off-specular peak direction
//R = GetOffSpecularPeakReflectionDir(N, R, GBuffer.Roughness);

DeferredOutPut MeshTexture_PS(Output _Input)
{
    DeferredOutPut Result = (DeferredOutPut)0;
    
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
    
    Result.PosTarget = _Input.VIEWPOSITION;    
    Result.DifTarget = float4(AlbmData.r, AlbmData.g, AlbmData.b, AtosData.r);
                
    float4 WorldNormal;
    
    if (0 != IsNormal)
    {
        Result.NorTarget = NormalTexCalculate(NormalTexture, ENGINEBASE, _Input.TEXCOORD, _Input.TANGENT, _Input.BINORMAL, _Input.NORMAL);
        WorldNormal = NormalTexCalculate(NormalTexture, ENGINEBASE, _Input.TEXCOORD, _Input.WORLDNORMAL, _Input.WORLDNORMAL, _Input.WORLDNORMAL);
    }
    else
    {
        Result.NorTarget = _Input.NORMAL;
        WorldNormal = _Input.WORLDNORMAL;
    }
         
    // �ݻ緮 ��� ���� �����Ͻ� ���� ���� �����ȴ�        
    float roughness = 1.0 - NrmrData.r; // smoothness�� �����Ͻ� ���Դϴ�.
    float3 reflection = reflect(AllLight[0].LightRevDir.xyz, Result.NorTarget.xyz); // ���� �ݻ� ���� ���
    float distribution = GGX_Distribution(Result.NorTarget.xyz, reflection, roughness); // �ݻ� ���� ���
        
    // View.TranslatedWorldCameraOrigin    
    float3 CameraPos = AllLight[0].CameraWorldPosition;
    // TranslatedWorldPosition
    float3 RefViewPos = _Input.WORLDPOSITION.xyz; 
        
    // CameraToPixel (World)
    float3 CameraView = normalize(CameraPos - RefViewPos);
    
    // �ݻ纤��
    float4 refnormal = normalize(WorldNormal);
    
    // ReflectionVector    
    float3 refvector;
    
    // Floor Water Noise Option
    if (0 != BaseColor.g)
    {
        refnormal.xyz = NoiseWaterNormal(_Input.TEXCOORD.xy, WaterNoiseTexture, ENGINEBASE);
        //refnormal.xyz = mul(refnormal, _Input.WORLDMATRIX).xyz;        
        //refnormal.z = -refnormal.z;
    }
    
    //refnormal.y = -refnormal.y;
    
    // Point lobe in off-specular peak direction 
    // �ڵ� ���� �ʿ�
    //refnormal.xyz = GetOffSpecularPeakReflectionDir(refnormal.xyz, CameraViewDir, roughness);
            
    refvector = CalReflection(normalize(refnormal.xyz), normalize(CameraView));
    // �� ����
        
    float4 ReflectionColor = ReflectionTexture.Sample(CUBEMAPSAMPLER, normalize(refvector));
    //float4 ReflectionColor = ReflectionTexture.Sample(ENGINEBASE, refvector);
    
    // ���� ��Ż�� ��
    float metallic = saturate(AlbmData.a - distribution);
     
    // sss (subsurface scattering)
    
    // AlbmData -> metallicValue ���� ���� �����Ǿ�� �Ѵ�        
    Result.DifTarget.rgb = lerp(AlbmData.rgb, AlbmData.rgb * 0.3f, metallic);
    Result.DifTarget.rgb += lerp(float3(0, 0, 0), ReflectionColor.rgb * 0.7f, metallic);
    
    Result.DifTarget.a = 1.0f;
    Result.PosTarget.a = 1.0f;
    Result.NorTarget.a = 1.0f;    
    
    Result.MatTarget.r = metallic;
    Result.MatTarget.g = roughness;
    Result.MatTarget.a = 1.0f;
    
    
    Result.SSSTarget.r = AtosData.r;
    Result.SSSTarget.g = AtosData.g;
    Result.SSSTarget.b = AtosData.b;
    Result.SSSTarget.a = 1.0f;
    
    if(0 != BaseColor.r)
    {
        Result.GlamTarget.rgb = AlbmData.rgb * BaseColor.r;
    }
       
    Result.GlamTarget.a = 1.0f;
        
    return Result;
}