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


// Ό³Έν :
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
	void ParticleRendererInit();

private:
	int MaxCount;
	std::shared_ptr<class GameEngineVertexBuffer> Buffer;
	std::shared_ptr<class GameEngineStructuredBuffer> m_ParticleBuffer;
};

