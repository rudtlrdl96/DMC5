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
	Result->Nav = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->Nav->SetFBXMesh(_FBXName.data(), "FBX");
	Result->Nav->Off();

	float4 RenderScale = Result->Nav->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 MeshScale = Result->Nav->GetMeshScale();

	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };

	Result->Colider = Result->CreateComponent<PhysXTriangleComponent>(); // 콜라이더 생성
	Result->Colider->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Result->Colider->CreatePhysXActors(_FBXName.data(), _Level->GetLevelScene(), _Level->GetLevelPhysics(), _Level->GetCooking(), false, 0.01f);

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
