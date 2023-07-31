#include "PrecompileHeader.h"
#include "GameEngineFBXMesh.h"

GameEngineFBXMesh::GameEngineFBXMesh() 
{
}

GameEngineFBXMesh::~GameEngineFBXMesh() 
{
}


std::shared_ptr<GameEngineFBXMesh> GameEngineFBXMesh::Load(const std::string& _Path, const std::string& _Name)
{
	std::shared_ptr<GameEngineFBXMesh> NewTexture = GameEngineResource::Create(_Name);
	return NewTexture;
}