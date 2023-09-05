#include "PrecompileHeader.h"
#include "NavMesh.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXTriangleComponent.h>

NavMesh::NavMesh()
{

}

NavMesh::~NavMesh()
{

}

std::shared_ptr<NavMesh> NavMesh::CreateNavMesh(GameEngineLevel* _Level, const std::string_view& _FBXName)
{
	std::shared_ptr<NavMesh> Result;
	Result = _Level->CreateActor<NavMesh>(); // 렌더러 생성
	Result->SetName("NavMesh");
	Result->Nav = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->Nav->SetName("Renderer");
	Result->Nav->SetFBXMesh(_FBXName.data(), "FBX");
	Result->Nav->Off();

	float Ratio = 1.f;

	Result->Collider = Result->CreateComponent<PhysXTriangleComponent>(); // 콜라이더 생성
	Result->Collider->SetName("Collider");
	Result->Collider->SetPhysxMaterial(1.0f, 1.0f, 0.05f);
	Result->Collider->SetGroundObject();
	Result->Collider->CreatePhysXActors(_FBXName.data(), true, Ratio);

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
