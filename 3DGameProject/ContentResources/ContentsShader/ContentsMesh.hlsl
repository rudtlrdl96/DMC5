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
    float4 WVPPOSITION : POSITION5;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
};

Output MeshTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
        
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    NewOutPut.WVPPOSITION = NewOutPut.POSITION;
    
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
TextureCube ReflectionTexture : register(t3); // Reflection Cubemap
Texture2D WaterNoiseTexture : register(t4); // WaterNoise

SamplerState ENGINEBASE : register(s0);

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
            
    if (0 != IsNormal)
    {
       Result.NorTarget = NormalTexCalculate(NormalTexture, ENGINEBASE, _Input.TEXCOORD, _Input.TANGENT, _Input.BINORMAL, _Input.NORMAL);
    }
    else
    {
        Result.NorTarget = _Input.NORMAL;
    }
        
    // �ݻ緮 ��� ���� �����Ͻ� ���� ���� �����ȴ�        
    float roughness = 1.0 - NrmrData.r; // smoothness�� �����Ͻ� ���Դϴ�.
    float3 reflection = reflect(AllLight[0].LightRevDir.xyz, Result.NorTarget.xyz); // ���� �ݻ� ���� ���
    float distribution = GGX_Distribution(Result.NorTarget.xyz, reflection, roughness); // �ݻ� ���� ���
        
    // View.TranslatedWorldCameraOrigin    
    float3 CameraPos = AllLight[0].CameraPosition;
    // TranslatedWorldPosition
    float3 RefViewPos = _Input.VIEWPOSITION.xyz;    
    // CameraToPixel
    float3 CameraView = normalize(CameraPos - RefViewPos);
    
    // �ݻ纤��
    float3 refnormal = Result.NorTarget.xyz;
                
    if (0 != BaseColor.g)
    {
        refnormal.xyz = NoiseWaterNormal(_Input.TEXCOORD.xy, WaterNoiseTexture, ENGINEBASE);
    }
    
    // Point lobe in off-specular peak direction
    refnormal = GetOffSpecularPeakReflectionDir(refnormal, CameraView, roughness);
    
    // ReflectionVector    
    float3 refvector = mul(float4(reflect(refnormal, CameraView), 0.0f), AllLight[0].CameraViewInverseMatrix).xyz;
    
    // �� ����
    //refvector.x = -refvector.x;
        
    float4 ReflectionColor = ReflectionTexture.Sample(ENGINEBASE, normalize(refvector));
    //float4 ReflectionColor = ReflectionTexture.Sample(ENGINEBASE, refvector);
    
    // ���� ��Ż�� ��
    float metallic = saturate(AlbmData.a - distribution);
     
    // sss (subsurface scattering)
    
    // AlbmData -> metallicValue ���� ���� �����Ǿ�� �Ѵ�        
    Result.DifTarget.rgb = lerp(AlbmData.rgb, AlbmData.rgb * 0.5f, metallic);
    Result.DifTarget.rgb += lerp(float3(0, 0, 0), ReflectionColor.rgb * 0.5f, metallic);
    
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