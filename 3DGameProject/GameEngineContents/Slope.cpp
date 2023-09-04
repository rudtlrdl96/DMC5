#include "PrecompileHeader.h"
#include "Slope.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "MyContentSpriteRenderer.h"
#include <GameEngineCore/PhysXBoxComponent.h>
#include <GameEngineCore/GameEngineRenderer.h>

Slope::Slope() 
{
}

Slope::~Slope() 
{
}

void Slope::Start()
{
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Ground_Mesh.FBX", "MeshTexture");
	Renderer->LightOff();

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z };

	Component = CreateComponent<PhysXBoxComponent>();
	//Component->SetPhysxMaterial(1.f, 3.5f, 0.1f);
	Component->SetPhysxMaterial(1.0f, 1.0f, 0.0f);
	Component->CreatePhysXActors(GeoMetryScale);
	Component->SetPositionSetFromParentFlag(true);
}

void Slope::Update(float _DeltaTime)
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

void Slope::UserUpdate(float _DeltaTime)
{

}

void Slope::ServerUpdate(float _DeltaTime)
{

}