#include "PrecompileHeader.h"
#include "NavMesh.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

NavMesh::NavMesh()
{

}

NavMesh::~NavMesh()
{

}

std::shared_ptr<NavMesh> NavMesh::CreateNavMesh(GameEngineLevel* _Level, const std::string_view& _FBXName)
{
	std::shared_ptr<NavMesh> Result;
	Result = _Level->CreateActor<NavMesh>();
	Result->Nav = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->Nav->SetFBXMesh(_FBXName.data(), "FBX");
	Result->Nav->Off();
	return Result;
}

void NavMesh::NavRenderOn()
{
	Nav->On();
}

void NavMesh::NavRenderOff()
{
	Nav->Off();
}

bool NavMesh::NavRenderIsUpdate()
{
	return Nav->IsUpdate();
}

void NavMesh::Start()
{
}

void NavMesh::Update(float _DeltaTime)
{
}
