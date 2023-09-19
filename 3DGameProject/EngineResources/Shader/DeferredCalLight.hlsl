#include "Light.fx"

struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
};

struct Output
{
    float4 POSITION : SV_Position;
    float4 TEXCOORD : TEXCOORD;
};

// 빛계산을 통해서 빛의 반사를 구현하고 나면
// 그 빛을 계산하는 시점에 따라서 예전에는 구분을 했다.
// 이제는 구분이 거의 의미가 없다.
// 빛이라는 컬러를 구해내는 것이 된다.

// 그걸 버텍스 쉐이더에서하면 그걸 점단위로 하면 플랫쉐이딩
// 그걸 버텍스 쉐이더에서하면 그걸 고로쉐이딩
// 그걸 픽셀 쉐이더에서하면 그걸 퐁쉐이딩

// 그래픽카드에서 이뤄지는것.
Output DeferredCalLight_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    NewOutPut.POSITION = _Input.POSITION;
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    return NewOutPut;
}

struct LightOutPut
{
    float4 DifLight : SV_Target0;
    float4 SpcLight : SV_Target1;
    float4 AmbLight : SV_Target2;
};

Texture2D PositionTex : register(t0); //rgb = pos
Texture2D NormalTex : register(t1); // rgb = normal
Texture2D MatTex : register(t2); // r = metal, g = roughness
Texture2D GleamTex : register(t3); // rgb = Gleam light
Texture2D ShadowTex : register(t4);

SamplerState POINTWRAP : register(s0);

float GGX_Distribution(float3 normal, float3 halfVector, float roughness)
{
    float NdotH = max(dot(normal, halfVector), 0.0f);
    float roughnessSqr = roughness * roughness;
    float a = roughnessSqr * roughnessSqr;
    float denominator = (NdotH * NdotH * (a - 1.0f) + 1.0f);
    return a / (3.14f * denominator * denominator);
}

LightOutPut DeferredCalLight_PS(Output _Input) : SV_Target0
{
    LightOutPut NewOutPut = (LightOutPut) 0;
    
    float4 Position = PositionTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);        
    float4 Normal = NormalTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    float4 Mat = MatTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
    float4 Gleam = GleamTex.Sample(POINTWRAP, _Input.TEXCOORD.xy);
            
    if (0 == Position.z)
    {
        clip(-1);
    }
    
    float4 DiffuseRatio = (float4) 0.0f;
    float4 SpacularRatio = (float4) 0.0f;
    float4 AmbientRatio = (float4) 0.0f;
                                          
    for (int i = 0; i < LightCount; ++i)
    {
        float4 CurLightDiffuseRatio = (float4) 0.0f;
        float4 CurLightSpacularRatio = (float4) 0.0f;
        
        float LightPower = AllLight[i].LightPower;
        
        if (0 != AllLight[i].LightType)
        {
            float Distance = length(AllLight[i].ViewLightPos.xyz - Position.xyz);
            
            // 200
            float FallOffStart = AllLight[i].LightRange * 0.2f;
            
            // 1000
            float FallOffEnd = AllLight[i].LightRange;
            
            if (Distance > FallOffEnd)
            {
                continue;
            }
            
            LightPower *= saturate((FallOffEnd - Distance) / (FallOffEnd - FallOffStart));
        }
        
        if (2 == AllLight[i].LightType)
        {
            float3 LightVec = normalize(AllLight[i].ViewLightPos.xyz - Position.xyz);
            float3 SpotCone = pow(saturate(dot(LightVec, normalize(AllLight[i].ViewLightDir.xyz))), AllLight[i].LightAngle);
            
            LightPower *= SpotCone;
        }
        
        if (0.0f < LightPower)
        {
            // Diffuse Light 계산
            CurLightDiffuseRatio.xyz = AllLight[i].LightColor.xyz * CalDiffuseLight(Position, Normal, AllLight[i]).xyz * LightPower;
        
            // Spacular Light 계산
            CurLightSpacularRatio.xyz = AllLight[i].LightColor.xyz * CalSpacularLight(Position, Normal, AllLight[i]).xyz * (1.0f - Mat.r) * LightPower * 0.5f;
        }        
        
        
        if (CurLightDiffuseRatio.x > 0.0f)
        {
            if (1 == AllLight[i].LightType)
            {
                continue;
            }
            
            float4 WorldViewPos = Position;
            WorldViewPos.a = 1.0f;
            // 빛이존재하므로
            // 그림자도 존재해야할지 판단한다.
            // 어느 world 
            float4 WorldPos = mul(WorldViewPos, AllLight[i].CameraViewInverseMatrix);
        
            // 빛을 기준으로한 포지션으로 바꿨다.
            float4 LightPos = mul(WorldPos, AllLight[i].LightViewProjectionMatrix);
        
            // worldviewprojection 
            // 이 곱해지면 그건 -1~1사이의 공간입니까?
            // w에 곱해지기전의 z값을 보관해 놓은 값이 됩니다.
            float3 LightProjection = LightPos.xyz / LightPos.w;
            // 모든 값은 -1~1사이의 값이 됩니다.
        
            float2 ShadowUV = float2(LightProjection.x * 0.5f + 0.5f, LightProjection.y * -0.5f + 0.5f);
            float fShadowDepth = ShadowTex.Sample(POINTWRAP, float2(ShadowUV.x, ShadowUV.y)).r;
        
            // 가장 외각을 약간 깎아내서 
            if (
            0.001f < ShadowUV.x && 0.999f > ShadowUV.x
            && 0.001f < ShadowUV.y && 0.999f > ShadowUV.y
            && LightProjection.z >= (fShadowDepth + 0.001f)
            )
            {
                CurLightDiffuseRatio *= 0.01f;
                CurLightSpacularRatio *= 0.01f;
            }
        }
        
        DiffuseRatio += CurLightDiffuseRatio;
        SpacularRatio += CurLightSpacularRatio;
    }
    
    SpacularRatio += float4(Gleam.r, Gleam.g, Gleam.b, 0);
    
    // 개선 여지 있음. 
    AmbientRatio = AllLight[0].AmbientLight;
    
    DiffuseRatio.a = 1.0f;
    SpacularRatio.a = 1.0f;
    AmbientRatio.a = 1.0f;
    
    NewOutPut.DifLight = DiffuseRatio;
    NewOutPut.SpcLight = SpacularRatio;
    NewOutPut.AmbLight = AmbientRatio;
    
    return NewOutPut;
}
