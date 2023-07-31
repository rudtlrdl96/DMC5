#pragma once
#include "GameEngineResource.h"
#include <vector>
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"

// 설명 :
class GameEngineMesh : public GameEngineResource<GameEngineMesh>
{
	friend class GameEngineRenderUnit;

public:
	static std::shared_ptr<GameEngineMesh> Create(const std::string_view& _Name)
	{
		return Create(_Name, _Name, _Name);
	}

	static std::shared_ptr<GameEngineMesh> Create(const std::string_view& _Name, const std::string_view& _VtxName, const std::string_view& _IdxName)
	{
		std::shared_ptr<GameEngineMesh> Res = GameEngineResource::Create(_Name);
		Res->VertexBufferPtr = GameEngineVertexBuffer::Find(_VtxName);
		Res->IndexBufferPtr = GameEngineIndexBuffer::Find(_IdxName);

		if ((nullptr == Res->VertexBufferPtr) || (nullptr == Res->IndexBufferPtr))
		{
			MsgAssert("매쉬를 만드는데 실패했습니다.");
		}

		return Res;
	}

	std::shared_ptr<GameEngineVertexBuffer> GetVertexBuffer() 
	{
		return VertexBufferPtr;
	}

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _TOPOLOGY)
	{
		TOPOLOGY= _TOPOLOGY;
	}

	// constrcuter destructer
	GameEngineMesh();
	~GameEngineMesh();

	// delete Function
	GameEngineMesh(const GameEngineMesh& _Other) = delete;
	GameEngineMesh(GameEngineMesh&& _Other) noexcept = delete;
	GameEngineMesh& operator=(const GameEngineMesh& _Other) = delete;
	GameEngineMesh& operator=(GameEngineMesh&& _Other) noexcept = delete;

protected:
	void Setting() override;

	void InputAssembler1();
	void InputAssembler2();

private:
	D3D11_PRIMITIVE_TOPOLOGY TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	std::shared_ptr<GameEngineVertexBuffer> VertexBufferPtr;
	std::shared_ptr<GameEngineIndexBuffer> IndexBufferPtr;
};

