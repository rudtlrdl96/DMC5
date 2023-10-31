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
Texture2D PaperBurnTexture : register(t5); // PaperBurn

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
    float4 WorldPosTarget : SV_Target6;
    float4 MaskTarget : SV_Target7;
};

DeferredOutPut MeshTexture_PS(Output _Input)
{
    DeferredOutPut Result = (DeferredOutPut)0;
    
    // rgb = 색상, a = metallicValue 
    float4 AlbmData = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // rgb = NormalMap, a = smoothnessValue 
    float4 NrmrData = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // r = Alpha, gba = sss (subsurface scattering)
    float4 AtosData = SpecularTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    if (0 != BaseColor.b)
    {
        float4 BurnTexData = PaperBurnTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
        
        if (BurnTexData.r < BaseColor.b)
        {
            clip(-1);
        }
                
        if (BurnTexData.r <= BaseColor.b + 0.01)
        {
            AlbmData = float4(0.6, 0, 0, 1);
        }
        else if (BurnTexData.r <= BaseColor.b + 0.02)
        {
            AlbmData = float4(0.8, 0, 0, 1);
        }
        else if (BurnTexData.r <= BaseColor.b + 0.05)
        {
            AlbmData = float4(1, 0, 0, 1); // 빨
        }
    }    
                   
    if (1.0f != AtosData.r)
    {
        clip(-1);
    }
    
    Result.PosTarget = _Input.VIEWPOSITION;    
    Result.WorldPosTarget = _Input.WORLDPOSITION;
    Result.DifTarget = float4(AlbmData.r, AlbmData.g, AlbmData.b, 1.0f);
                
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
         
    // 반사량 계산 공식 러프니스 값에 따라서 결정된다        
    float roughness = 1.0 - NrmrData.r; // smoothness는 러프니스 값입니다.
    float3 reflection = reflect(AllLight[0].LightRevDir.xyz, Result.NorTarget.xyz); // 빛의 반사 방향 계산
    float distribution = GGX_Distribution(Result.NorTarget.xyz, reflection, roughness); // 반사 분포 계산
        
    // View.TranslatedWorldCameraOrigin    
    float3 CameraPos = AllLight[0].CameraWorldPosition;
    // TranslatedWorldPosition
    float3 RefViewPos = _Input.WORLDPOSITION.xyz; 
        
    // CameraToPixel (World)
    float3 CameraView = normalize(CameraPos - RefViewPos);
    
    // 반사벡터
    float4 refnormal = normalize(WorldNormal);
    
    // ReflectionVector    
    float3 refvector;
    
    // Floor Water Noise Option
    if (0 != BaseColor.g)
    {
        refnormal.xyz = NoiseWaterNormal(_Input.TEXCOORD.xy, WaterNoiseTexture, ENGINEBASE);
    }
        
    // Point lobe in off-specular peak direction 
    // 코드 수정 필요
    //refnormal.xyz = GetOffSpecularPeakReflectionDir(refnormal.xyz, CameraViewDir, roughness);
            
    refvector = CalReflection(normalize(refnormal.xyz), normalize(CameraView));
        
    float4 ReflectionColor = ReflectionTexture.Sample(CUBEMAPSAMPLER, normalize(refvector));
    
    // 계산된 메탈릭 값
    float metallic = saturate(AlbmData.a - distribution);
     
    // sss (subsurface scattering)
    
    // AlbmData -> metallicValue 값에 따라서 결정되어야 한다        
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
        Result.GlamTarget.rgb = Result.DifTarget.rgb * BaseColor.r;
    }
       
    Result.GlamTarget.a = 1.0f;
    
    if (0 != IsBlurMask)
    {
        Result.MaskTarget = Result.DifTarget;
    }
    else
    {
        Result.MaskTarget = float4(0, 0, 0, 0);
    }
        
    return Result;
}