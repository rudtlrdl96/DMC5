#include "PrecompileHeader.h"
#include "Ramp.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXTriangleComponent.h>

Ramp::Ramp() 
{
}

Ramp::~Ramp() 
{
}

void Ramp::Start()
{
	GetTransform()->SetWorldPosition(float4{ 200, 200, 200 });

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("SoccerBallMesh.fbx", "MeshTexture");
	Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });

	float4 RenderScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 MeshScale = Renderer->GetMeshScale();

	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };

	float4 world = GetTransform()->GetWorldScale();
	float4 local = GetTransform()->GetLocalScale();

	Component = CreateComponent<PhysXTriangleComponent>();
	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	//Component->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), VecSclae * 0.1f);
}
void Ramp::Update(float _DeltaTime)
{

}

void Ramp::UserUpdate(float _DeltaTime)
{

}
void Ramp::ServerUpdate(float _DeltaTime)
{

}