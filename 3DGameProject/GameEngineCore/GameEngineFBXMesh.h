#pragma once
#include "GameEngineResource.h"
#include "GameEngineFBX.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineTexture.h"
#include <GameEngineBase/GameEnginePath.h>

// fbx는 오토데스크

// 버텍스 버퍼 만드는 용도
// 설명 :
class GameEngineFBXMesh : public GameEngineFBX, public GameEngineResource<GameEngineFBXMesh>
{
public:
	// constrcuter destructer
	GameEngineFBXMesh();
	~GameEngineFBXMesh();

	// delete Function
	GameEngineFBXMesh(const GameEngineFBXMesh& _Other) = delete;
	GameEngineFBXMesh(GameEngineFBXMesh&& _Other) noexcept = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh& _Other) = delete;
	GameEngineFBXMesh& operator=(GameEngineFBXMesh&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineFBXMesh> Load(const std::string& _Path)
	{
		GameEnginePath FilePath(_Path);

		return Load(_Path, FilePath.GetFileName());
	}

	static std::shared_ptr<GameEngineFBXMesh> Load(const std::string& _Path, const std::string& _Name);

protected:

private:

};

