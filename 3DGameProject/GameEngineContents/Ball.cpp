#include "PrecompileHeader.h"
#include "Ball.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXSphereComponent.h>

Ball::Ball() 
{
}

Ball::~Ball() 
{
}

void Ball::Start()
{
	GetTransform()->SetWorldPosition(float4{-200, 200, 200});

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("SoccerBallMesh.fbx", "MeshTexture");
	Renderer->GetTransform()->SetLocalScale({ 0.1f , 0.1f , 0.1f });

	float4 RenderScale = Renderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	float4 MeshScale = Renderer->GetMeshScale();

	physx::PxVec3 VecSclae = { RenderScale.x, RenderScale.y, RenderScale.z };

	float4 world = GetTransform()->GetWorldScale();
	float4 local = GetTransform()->GetLocalScale();

	Component = CreateComponent<PhysXSphereComponent>();
	Component->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	Component->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), VecSclae * 0.2f);
}
void Ball::Update(float _DeltaTime)
{

}

void Ball::UserUpdate(float _DeltaTime)
{

}
void Ball::ServerUpdate(float _DeltaTime)
{

}