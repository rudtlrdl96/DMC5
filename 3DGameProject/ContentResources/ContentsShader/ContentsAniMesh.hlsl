
#include "Transform.fx"
#include "Light.fx"
#include "RenderBaseValue.fx"
#include "Reflection.fx"
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
    float4 WORLDPOSITION : POSITION1;
    float4 VIEWPOSITION : POSITION2;
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

float GGX_Distribution(float3 normal, float3 halfVector, float roughness)
{
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float roughnessSqr = roughness * roughness;
    float a = roughnessSqr * roughnessSqr;
    float denominator = (NdotH * NdotH * (a - 1.0f) + 1.0f);
    return a / (3.14f * denominator * denominator);
}

float4 MeshAniTexture_PS(Output _Input) : SV_Target0
{
    // rgb = 색상, a = metallicValue 
    float4 AlbmData = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // rgb = NormalMap, a = smoothnessValue 
    float4 NrmrData = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    // r = Alpha, gba = sss
    float4 AtosData = SpecularTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    if (AlbmData.x == ClipColor.x && AlbmData.y == ClipColor.y && AlbmData.z == ClipColor.z)
    {
        clip(-1);
    }
        
    float4 RGBA = { AlbmData.r, AlbmData.g, AlbmData.b, AtosData.r };
    float4 ResultColor = float4(0, 0, 0, 0);
    
    if (0 != IsLight)
    {
        float4 NormalDir = _Input.NORMAL;
        
        if (0 != IsNormal)
        {
            NormalDir += NormalTexCalculate(NrmrData, _Input.TEXCOORD, _Input.TANGENT, _Input.BINORMAL, _Input.NORMAL);
        }
                
        // 반사량 계산 공식 러프니스 값에 따라서 결정된다        
        float roughness = 1.0 - NrmrData.r; // smoothness는 러프니스 값입니다.
        float3 reflection = reflect(AllLight[0].LightRevDir.xyz, NormalDir.xyz); // 빛의 반사 방향 계산
        float distribution = GGX_Distribution(NormalDir.xyz, reflection, roughness); // 반사 분포 계산
                               
        float metallic = saturate(AlbmData.a - distribution);
        
        // AlbmData -> metallicValue 값에 따라서 결정되어야 한다        
        RGBA.rgb = lerp(AlbmData.rgb, AlbmData.rgb * 0.6f, metallic);
        
        float A = RGBA.w;
        
        float4 AmbientRatio = AllLight[0].AmbientLight;
        
        for (int i = 0; i < LightCount; ++i)
        {
            float LightPower = AllLight[i].LightPower;
            
            if (0 != AllLight[i].LightType)
            {
                float Distance = length(AllLight[i].LightPos.xyz - _Input.WORLDPOSITION.xyz);
                
                float FallOffStart = AllLight[i].LightRange * 0.15f;
                float FallOffEnd = AllLight[i].LightRange;
                
                if (Distance > FallOffEnd)
                {
                    continue;
                }
                
                LightPower *= saturate((FallOffEnd - Distance) / (FallOffEnd - FallOffStart));
            }
            
            if (2 == AllLight[i].LightType)
            {
                float3 LightVec = normalize(AllLight[i].LightPos.xyz - _Input.WORLDPOSITION.xyz);
                float3 SpotCone = dot(LightVec, normalize(AllLight[i].LightDir.xyz));
                
                LightPower *= SpotCone;
            }
            
            if (0.0f < LightPower)
            {
                // Diffuse Light 계산
                float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, NormalDir, AllLight[i]);
            
                // Spacular Light 계산
                float4 SpacularRatio = CalSpacularLight(_Input.VIEWPOSITION, NormalDir, AllLight[i]) * (1.0f - AlbmData.a);
            
                ResultColor += ((RGBA * DiffuseRatio) + (AlbmData * SpacularRatio)) * LightPower;
            }
        }
        
        ResultColor += RGBA * AmbientRatio;
        ResultColor.a = A;
    }
            
    if (1.0f != BaseColor.a)
    {
        float Step = ((_Input.POSITION.x + (_Input.POSITION.y * 2)) % 5) + 1;
        ResultColor.a = 1.0f - ((Step / 5.0f) * (1.0f - BaseColor.a));
    }
    
    clip(1.0f - ResultColor.a);
            
    return ResultColor;
}