#include "PrecompileHeader.h"
#include "Plane.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "MyContentSpriteRenderer.h"
#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>

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
	//Renderer->SetClipColor(float4(1.0f, 0.0f, 0.0f));
	Renderer->LightOff();

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z};

	Component = CreateComponent<PhysXBoxComponent>();
	//Component->SetPhysxMaterial(0.5f, 5.0f, 0.1f);
	Component->SetPhysxMaterial(1.0f, 1.0f, 0.0f);
	Component->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), GeoMetryScale);
	//Component->SetPositionSetFromParentFlag(true);
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