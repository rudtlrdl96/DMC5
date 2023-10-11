#include "PrecompileHeader.h"
#include "Plane.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>
#include "MyContentSpriteRenderer.h"
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
	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Ground_Mesh.FBX", "FBX_Low");
	Renderer->GetTransform()->SetLocalScale(Renderer->GetTransform()->GetLocalScale() * 3.0f);
	//Renderer->LightOff();

	float4 RenderMeshScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox * 3.0f;
	physx::PxVec3 GeoMetryScale = { RenderMeshScale.x, RenderMeshScale.y, RenderMeshScale.z};

	Component = CreateComponent<PhysXBoxComponent>();
	Component->SetPhysxMaterial(1.0f, 1.0f, 0.0f);
	Component->SetGroundObject();
	Component->CreatePhysXActors(GeoMetryScale);
}

void Plane::Update(float _DeltaTime)
{
	
}

void Plane::UserUpdate(float _DeltaTime)
{

}

void Plane::ServerUpdate(float _DeltaTime)
{

}