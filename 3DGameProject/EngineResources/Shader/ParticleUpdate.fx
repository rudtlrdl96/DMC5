#include "RenderBaseValue.fx"

// Noise Texture
Texture2D EngineNoise : register(t15);

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

float4 GaussianSample(float2 _vUV)
{
    float4 vOutColor = (float4) 0.f;
    
    if (1.f < _vUV.x)
        _vUV.x = frac(_vUV.x);
    else if (_vUV.x < 0.f)
        _vUV.x = 1.f + frac(_vUV.x);
    
    if (1.f < _vUV.y)
        _vUV.y = frac(_vUV.y);
    else if (_vUV.y < 0.f)
        _vUV.x = 1.f + frac(_vUV.x);
    
    int2 iUV = _vUV * NoiseResolution.xy;
    iUV -= int2(2, 2);
    
    for (int j = 0; j < 5; ++j)
    {
        for (int i = 0; i < 5; ++i)
        {
            int2 idx = int2(iUV.y + i, iUV.x + j);
            vOutColor += EngineNoise[idx] * GaussianFilter[j][i];
        }
    }
    
    return vOutColor;
}


cbuffer ParticleUpdateInfo : register(b7)
{
    int PARTICLE_MAX;
    int IsWorldSpawn;
    float SpawnRange;
    float MinSpeed;
    float MaxSpeed;
    float MinLife;
    float MaxLife;
    float Temp1;
    float4 ObjectWorldPos; // 액터나 랜더러의 월드 포지션을 
};

struct ParticleComputeData
{
    // 현재 위치
    float4 vRelativePos;
    // 파티클의 방향
    float4 vDir;
		
    // 시간 사라지는시간
    float fMaxTime;
    // 현재 시간
    float fCurTime;
    // 이동할때의 스피드
    float fSpeed;
    // 살아있냐 죽어있냐.
    uint iActive;
};

struct ParticleShareData
{
    uint iAliveCount;
    uint iPadding0;
    uint iPadding1;
    uint iPadding2;
};

// 어제 스트럭처드 버퍼중 UAV_INC <= 으로 만든 녀석만 RW스트럭처드 버퍼로 세팅이 가능하다.
RWStructuredBuffer<ParticleComputeData> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShareData> ParticleShare : register(u1);


//#define PARTICLE_MAX    g_int_0
//#define Particle        ParticleBuffer[_id.x]
//#define ShareData       ParticleShare[0]

//#define SpawnRange      g_float_0
//#define IsWorldSpawn    g_int_1
//#define ObjectWorldPos  g_vec4_0
//#define MinSpeed        g_vec2_0.x
//#define MaxSpeed        g_vec2_0.y
//#define MinLife         g_vec2_1.x
//#define MaxLife         g_vec2_1.y



[numthreads(128, 1, 1)] 
void ParticleUpdate_CS(uint3 _id : SV_DispatchThreadID)
{
    // _id쓰레드 넘버
    
    if ((uint) PARTICLE_MAX <= _id.x)
        return;
            
    if (0 == ParticleBuffer[_id.x].iActive)
    {   
        // 
        while (0 < ParticleShare[0].iAliveCount)
        {
            int iOriginValue = ParticleShare[0].iAliveCount;
            int iExchange = iOriginValue - 1;
        
            //InterlockedExchange(ShareData.iAliveCount, iExchange, iExchange);
            // ParticleShare[0].iAliveCount 모든 그래픽카드의 쓰레드에서 공유하는 정수라는 거네요.
            InterlockedCompareExchange(ParticleShare[0].iAliveCount, iOriginValue, iExchange, iExchange);
            
            if (iOriginValue == iExchange)
            {
                ParticleBuffer[_id.x].iActive = 1;
                break;
            }
        }
        
        if (ParticleBuffer[_id.x].iActive)
        {
            // 랜덤값으로 위치와 방향을 설정한다.
            // 샘플링을 시도할 UV 를 계산한다.
            float4 vRandom = (float4) 0.f;
            
            float2 vUV = float2((float) _id.x / PARTICLE_MAX, 0.5f);    
            vUV.x += SumDeltaTime;
            vUV.y += sin((vUV.x + SumDeltaTime) * 3.141592f * 2.f * 10.f) * 0.5f;
            
            vRandom = float4(GaussianSample(vUV + float2(0.f, 0.f)).x, GaussianSample(vUV + float2(0.1f, 0.f)).x, GaussianSample(vUV + float2(0.2f, 0.f)).x, GaussianSample(vUV + float2(0.3f, 0.f)).x);
            
            
            // 사각형범위로 스폰
            // Particle.vRelativePos.xyz = vRandom.xyz * SpawnRange - SpawnRange / 2.f;
            
            // 원형 범위로 스폰
            float fTheta = vRandom.x * 3.141592f * 2.f;
            ParticleBuffer[_id.x].vRelativePos.xy = float2(cos(fTheta), sin(fTheta)) * vRandom.y * SpawnRange;
            ParticleBuffer[_id.x].vRelativePos.z = 0.f;
            
            // 파티클 방향
            ParticleBuffer[_id.x].vDir.xy = -normalize(float2(ParticleBuffer[_id.x].vRelativePos.xy));
            
            if (IsWorldSpawn)
            {
                ParticleBuffer[_id.x].vRelativePos.xyz += ObjectWorldPos.xyz;
            }
            
            // 파티클 속력
            ParticleBuffer[_id.x].fSpeed = vRandom.z * (MaxSpeed - MinSpeed) + MinSpeed;
            
            // 파티클 Life
            ParticleBuffer[_id.x].fCurTime = 0.f;
            ParticleBuffer[_id.x].fMaxTime = 2.f; //vRandom.w * (MaxLife - MinLife) + MinLife;
        }
    }    
    
    // 활성화 된 파티클을 업데이트
    else 
    {
        ParticleBuffer[_id.x].fCurTime += DeltaTime;
        if (ParticleBuffer[_id.x].fMaxTime < ParticleBuffer[_id.x].fCurTime)
        {
            ParticleBuffer[_id.x].iActive = 0;
        }
        else
        {
            ParticleBuffer[_id.x].vRelativePos += ParticleBuffer[_id.x].vDir * ParticleBuffer[_id.x].fSpeed * DeltaTime;
        }
    }        
}

//#endif
