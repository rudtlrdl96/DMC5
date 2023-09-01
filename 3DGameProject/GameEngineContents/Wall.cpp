#include "PrecompileHeader.h"
#include "Wall.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "MyContentSpriteRenderer.h"
#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/GameEngineRenderer.h>

Wall::Wall() 
{
}

Wall::~Wall() 
{
}

void Wall::Start()
{
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Ground_Mesh.FBX", "MeshTexture");
	//Renderer->SetClipColor(float4(1.0f, 0.0f, 0.0f));

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z };

	Component = CreateComponent<PhysXBoxComponent>();
	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Component->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), GeoMetryScale);
	Component->SetPositionSetFromParentFlag(true);
}

void Wall::Update(float _DeltaTime)
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

void Wall::UserUpdate(float _DeltaTime)
{

}

void Wall::ServerUpdate(float _DeltaTime)
{

}