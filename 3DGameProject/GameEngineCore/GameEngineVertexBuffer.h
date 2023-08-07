#pragma once
#include "GameEngineResource.h"
#include "GameEngineVertex.h"
#include "GameEngineDirectBuffer.h"

// Ό³Έν :
class GameEngineVertexBuffer : public GameEngineResource<GameEngineVertexBuffer>, public GameEngineDirectBuffer
{
	friend class GameEngineInputLayOut;

public:
	// constrcuter destructer
	GameEngineVertexBuffer();
	~GameEngineVertexBuffer();

	// delete Function
	GameEngineVertexBuffer(const GameEngineVertexBuffer& _Other) = delete;
	GameEngineVertexBuffer(GameEngineVertexBuffer&& _Other) noexcept = delete;
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer& _Other) = delete;
	GameEngineVertexBuffer& operator=(GameEngineVertexBuffer&& _Other) noexcept = delete;

	template<typename VertexType>
	static std::shared_ptr<GameEngineVertexBuffer> Create(const std::string_view& _Name, const std::vector<VertexType>& _Vertexs)
	{
		std::shared_ptr<GameEngineVertexBuffer> Res = GameEngineResource::Create(_Name);
		Res->LayOutInfo = &VertexType::LayOut;
		Res->ResCreate(&_Vertexs[0], sizeof(VertexType), static_cast<UINT>(_Vertexs.size()));
		return Res;
	}

	template<typename VertexType>
	static std::shared_ptr<GameEngineVertexBuffer> Create(const std::vector<VertexType>& _Vertexs)
	{
		std::shared_ptr<GameEngineVertexBuffer> Res = GameEngineResource::CreateUnNamed();
		Res->LayOutInfo = &VertexType::LayOut;
		Res->ResCreate(&_Vertexs[0], sizeof(VertexType), static_cast<UINT>(_Vertexs.size()));
		return Res;
	}


	void Setting() override;

protected:

private:
	GameEngineInputLayOutInfo* LayOutInfo = nullptr;

	void ResCreate(const void* _Data, UINT _VertexSize, UINT _VertexCount);
	UINT Offset = 0;
	UINT VertexSize = 0;
	UINT VertexCount = 0;
};

