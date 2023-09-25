#include "PrecompileHeader.h"
#include "Box200.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>

Box200::Box200() 
{
}

Box200::~Box200() 
{
}

void Box200::Start()
{
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Box_350x250x200_Mesh.FBX", "FBX_Low");
	Renderer->GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });
	//Renderer->LightOff();

	//float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 RenderMeshScale = Renderer->GetMeshScale();
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z };

	Component = CreateComponent<PhysXBoxComponent>();
	Component->SetPhysxMaterial(1.0f, 1.0f, 0.0f);
	Component->SetPositionSetFromParentFlag(true);
	Component->SetObstacleObject();
	Component->CreatePhysXActors(GeoMetryScale * 0.5f);

	//GameEngineInput::CreateKey("Select_01", 'Y');
	//GameEngineInput::CreateKey("Select_02", 'U');
}

void Box200::Update(float _DeltaTime)
{

}

