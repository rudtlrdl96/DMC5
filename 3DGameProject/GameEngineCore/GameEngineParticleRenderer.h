#pragma once
#include "GameEngineRenderer.h"

struct ParticleData
{
	float4 vRelativePos;
	float4 vDir;

	float fMaxTime;
	float fCurTime;
	float fSpeed;
	UINT  iActive;
};

struct ParticleUpdateInfo
{
	int PARTICLE_MAX = 1000;
	int IsWorldSpawn = 1;
	float SpawnRange = 300.0f;
	float MinSpeed = 0.0f;
	float MaxSpeed = 0.0f;
	float MinLife = 1.0f;
	float MaxLife = 5.0f;
	float Temp1;
	float4 ObjectWorldPos; // ���ͳ� �������� ���� �������� 
};


struct ParticleComputeData
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

struct ParticleShareData
{
	uint iAliveCount;
	uint iPadding0;
	uint iPadding1;
	uint iPadding2;
};


// ���� :
class GameEngineParticleRenderer : public GameEngineRenderer
{
public:
	// constrcuter destructer
	GameEngineParticleRenderer();
	~GameEngineParticleRenderer();

	// delete Function
	GameEngineParticleRenderer(const GameEngineParticleRenderer& _Other) = delete;
	GameEngineParticleRenderer(GameEngineParticleRenderer&& _Other) noexcept = delete;
	GameEngineParticleRenderer& operator=(const GameEngineParticleRenderer& _Other) = delete;
	GameEngineParticleRenderer& operator=(GameEngineParticleRenderer&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void ParticleRendererInit();

	GameEngineComputeUnit ComUnit;
	ParticleUpdateInfo ParticleUpdateInfoValue;

private:
	int MaxCount;
	std::shared_ptr<class GameEngineVertexBuffer> Buffer;
	std::shared_ptr<class GameEngineStructuredBuffer> m_ParticleBuffer;

	std::shared_ptr<class GameEngineStructuredBuffer> ParticleBuffer;
	std::shared_ptr<class GameEngineStructuredBuffer> ParticleShare;

};

