#include "Transform.fx"

cbuffer ParticleInfo : register(b7)
{
    float4 StartScale;
    float4 EndScale;
    float4 StartColor;
    float4 EndColor;
};

struct ParticleData
{
    // ���� ��ġ
    float4 vRelativePos;
    // ��ƼŬ�� ����
    float4 vDir;
		
    // �ð� ������½ð�
    float fMaxTime;
    // ���� �ð�
    float fCurTime;
    // �̵��Ҷ��� ���ǵ�
    float fSpeed;
    // ����ֳ� �׾��ֳ�.
    uint iActive;
};

// ��Ʈ��ó�� ���۸� ����ϴ�.
// ������ŭ�� ���۰� �ʿ��ϴ�.
StructuredBuffer<ParticleData> ParticleBuffer : register(t16);

struct VS_IN
{
    float3 vPos : POSITION;
    // �̳༮�� ���� ���° ���ؽ� �������� ���ɴϴ�.
    uint iInstance : SV_InstanceID;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    uint iInstance : SV_InstanceID;
};

VS_OUT ParticleRender_VS(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vLocalPos = _in.vPos;
    output.iInstance = _in.iInstance;
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint iInstance : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_ParticleRender(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    if (0 == ParticleBuffer[_in[0].iInstance].iActive)
        return;
   
    float3 vWorldPos = _in[0].vLocalPos + ParticleBuffer[_in[0].iInstance].vRelativePos.xyz;
    
    // ��ƼŬ ���̴��� ��������� �����ε�. 
    //if (0 == IsWorldSpawn)
    //{
    //    vWorldPos += g_matWorld._41_42_43;
    //}
    
    float3 vViewPos = mul(float4(vWorldPos, 1.f), View).xyz;

    
    // ���� ����
    float fRatio = ParticleBuffer[_in[0].iInstance].fCurTime / ParticleBuffer[_in[0].iInstance].fMaxTime;
    float3 vScale = lerp(StartScale.xyz, EndScale.xyz, fRatio);
    
    float3 NewPos[4] =
    {
        vViewPos - float3(-0.5f, 0.5f, 0.f) * vScale,
        vViewPos - float3(0.5f, 0.5f, 0.f) * vScale,
        vViewPos - float3(0.5f, -0.5f, 0.f) * vScale,
        vViewPos - float3(-0.5f, -0.5f, 0.f) * vScale
    };
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(float4(NewPos[i], 1.f), Projection);
        output[i].iInstance = _in[0].iInstance;
    }
    
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
       
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
}

Texture2D DiffuseTexture : register(t0);
SamplerState ENGINEBASE : register(s0);

float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = DiffuseTexture.Sample(ENGINEBASE, _in.vUV);
    
    float fRatio = ParticleBuffer[_in.iInstance].fCurTime / ParticleBuffer[_in.iInstance].fMaxTime;
    vColor.rgb *= lerp(StartColor, EndColor, fRatio).rgb;
    
    
    return vColor;

}