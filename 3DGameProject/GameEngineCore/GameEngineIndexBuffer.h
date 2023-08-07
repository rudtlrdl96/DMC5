#pragma once
#include "GameEngineResource.h"
#include "GameEngineDirectBuffer.h"

// ���� :
class GameEngineIndexBuffer : public GameEngineResource<GameEngineIndexBuffer>, public GameEngineDirectBuffer
{
public:
	// constrcuter destructer
	GameEngineIndexBuffer();
	~GameEngineIndexBuffer();

	// delete Function
	GameEngineIndexBuffer(const GameEngineIndexBuffer& _Other) = delete;
	GameEngineIndexBuffer(GameEngineIndexBuffer&& _Other) noexcept = delete;
	GameEngineIndexBuffer& operator=(const GameEngineIndexBuffer& _Other) = delete;
	GameEngineIndexBuffer& operator=(GameEngineIndexBuffer&& _Other) noexcept = delete;

	template<typename IndexType>
	static std::shared_ptr<GameEngineIndexBuffer> Create(const std::string_view& _Name, const std::vector<IndexType>& _Vertexs)
	{
		std::shared_ptr<GameEngineIndexBuffer> Res = GameEngineResource::Create(_Name);

		Res->ResCreate(&_Vertexs[0], sizeof(IndexType), static_cast<UINT>(_Vertexs.size()));

		return Res;
	}

	template<typename IndexType>
	static std::shared_ptr<GameEngineIndexBuffer> Create(const std::vector<IndexType>& _Indexs)
	{
		std::shared_ptr<GameEngineIndexBuffer> Res = GameEngineResource::CreateUnNamed();

		Res->ResCreate(&_Indexs[0], sizeof(IndexType), static_cast<UINT>(_Indexs.size()));

		return Res;
	}

	void Setting() override;

	inline UINT GetIndexCount()
	{
		return IndexCount;
	}

protected:

private:
	void ResCreate(const void* _Data, UINT _IndexSize, UINT _IndexCount);

	DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;

	UINT Offset = 0;
	UINT IndexSize = 0;
	UINT IndexCount = 0;
	UINT VertexCount = 0;
};

