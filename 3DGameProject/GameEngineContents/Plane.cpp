#include "PrecompileHeader.h"
#include "Plane.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "MyContentSpriteRenderer.h"
#include <GameEngineCore/PhysXBoxComponent.h>
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
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Ground_Mesh.FBX", "MeshTexture");

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z};

	Component = CreateComponent<PhysXBoxComponent>();
	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Component->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), GeoMetryScale);
	Component->SetPositionSetFromParentFlag(true);

	physx::PxTransform PhyTF = Component->GetDynamic()->getGlobalPose();
	PhyTF.q.z += 90.0f;

	//Component->GetDynamic()->setGlobalPose(PhyTF);
}

void Plane::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	switch (Type)
	{
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::NetControll:
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