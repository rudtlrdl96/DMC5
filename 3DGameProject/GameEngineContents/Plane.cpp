#include "PrecompileHeader.h"
#include "Plane.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "MyContentSpriteRenderer.h"
#include <GameEngineCore/PhysXBoxGeometryComponent.h>
#include <GameEngineCore/GameEngineRenderer.h>

#include "TestLevel.h"

Plane::Plane() 
{
}

Plane::~Plane() 
{
}

void Plane::Start()
{
	GetTransform()->AddWorldPosition(float4{0, -50.0f, 0});

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Ground_Mesh.FBX", "MeshTexture");

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z};

	Component = CreateComponent<PhysXBoxGeometryComponent>();
	Component->CreatePhysXActors(TestLevel::TestLevelPtr->GetLevelScene(), TestLevel::TestLevelPtr->GetLevelPhysics(), GeoMetryScale);
}

void Plane::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	switch (Type)
	{
	case NetControllType::None:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::ServerControll:
		ServerUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void Plane::UserUpdate(float _DeltaTime)
{

}

void Plane::ServerUpdate(float _DeltaTime)
{

}