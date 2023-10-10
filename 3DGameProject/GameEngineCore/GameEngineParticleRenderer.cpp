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

		// ��ǻƮ���̴� ���� ���۷� �����.
		// D3D11_BUFFER_DESC Buffer;
		m_ParticleBuffer->CreateResize(sizeof(ParticleData), MaxCount, StructuredBufferType::UAV_INC);
	}

}

void GameEngineParticleRenderer::ParticleRendererInit()
{

}