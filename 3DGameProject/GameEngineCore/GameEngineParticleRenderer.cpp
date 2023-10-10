#include "PrecompileHeader.h"
#include "GameEngineParticleRenderer.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineStructuredBuffer.h"

GameEngineParticleRenderer::GameEngineParticleRenderer()
{
}

GameEngineParticleRenderer::~GameEngineParticleRenderer()
{
}

void GameEngineParticleRenderer::Start()
{
	GameEngineRenderer::Start();

	MaxCount = 1000;

	// Buffer = GameEngineVertexBuffer::Create(Particle);

	{
		m_ParticleBuffer = std::make_shared<GameEngineStructuredBuffer>();

		// 컴퓨트쉐이더 계산용 버퍼로 만든다.
		// D3D11_BUFFER_DESC Buffer;
		m_ParticleBuffer->CreateResize(sizeof(ParticleData), MaxCount, StructuredBufferType::UAV_INC);
	}

}

void GameEngineParticleRenderer::ParticleRendererInit()
{

}