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

// ������� ���ؼ� ���� �ݻ縦 �����ϰ� ����
// �� ���� ����ϴ� ������ ���� �������� ������ �ߴ�.
// ������ ������ ���� �ǹ̰� ����.
// ���̶�� �÷��� ���س��� ���� �ȴ�.

// �װ� ���ؽ� ���̴������ϸ� �װ� �������� �ϸ� �÷����̵�
// �װ� ���ؽ� ���̴������ϸ� �װ� ��ν��̵�
// �װ� �ȼ� ���̴������ϸ� �װ� �����̵�

// �׷���ī�忡�� �̷����°�.
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

Texture2D PositionTex : register(t0); //rgb = pos, a = Materia
Texture2D NormalTex : register(t1); // rgb = normal, a = roughness
Texture2D MatTex : register(t2); // rgb = normal, a = roughness
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
            
    if(0 == Normal.z)
    {
        clip(-1);
    }
    
    float4 DiffuseRatio = (float4) 0.0f;
    float4 SpacularRatio = (float4) 0.0f;
    float4 AmbientRatio = (float4) 0.0f;
                                          
    for (int i = 0; i < LightCount; ++i)
    {
        float LightPower = AllLight[i].LightPower;
        
        if (0 != AllLight[i].LightType)
        {
            float Distance = length(AllLight[i].LightPos.xyz - Position.xyz);
            
            float FallOffStart = AllLight[i].LightRange * 0.15f;
            float FallOffEnd = AllLight[i].LightRange;
            
            if (Distance > FallOffEnd + FallOffStart)
            {
                continue;
            }
            
            LightPower *= saturate((FallOffEnd - Distance) / (FallOffEnd - FallOffStart));
        }
        
        if (2 == AllLight[i].LightType)
        {
            float3 LightVec = normalize(AllLight[i].LightPos.xyz - Position.xyz);
            float3 SpotCone = pow(saturate(dot(LightVec, normalize(AllLight[i].LightDir.xyz))), AllLight[i].LightAngle);
            
            LightPower *= SpotCone;
        }
        
        if (0.0f < LightPower)
        {
            // Diffuse Light ���
            DiffuseRatio.xyz += AllLight[i].LightColor.xyz * CalDiffuseLight(Position, Normal, AllLight[i]).xyz;
        
            // Spacular Light ���
            SpacularRatio.xyz += AllLight[i].LightColor.xyz * CalSpacularLight(Position, Normal, AllLight[i]).xyz * (1.0f - Mat.r);
        }        
    }
    
    // ���� ���� ����. 
    AmbientRatio = AllLight[0].AmbientLight;
    
    DiffuseRatio.a = 1.0f;
    SpacularRatio.a = 1.0f;
    AmbientRatio.a = 1.0f;
    
    NewOutPut.DifLight = DiffuseRatio;
    NewOutPut.SpcLight = SpacularRatio;
    NewOutPut.AmbLight = AmbientRatio;
    
    return NewOutPut;
}
