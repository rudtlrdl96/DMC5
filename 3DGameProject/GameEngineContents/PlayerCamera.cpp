#include "PrecompileHeader.h"
#include "PlayerCamera.h"

PlayerCamera::PlayerCamera() 
{
}

PlayerCamera::~PlayerCamera() 
{
}

void PlayerCamera::Start()
{
	GetTransform()->SetWorldRotation(float4::ZERO);

	CameraTarget = GetActor()->CreateComponent<GameEngineComponent>();
	TargetTransform = CameraTarget->GetTransform();
	TargetTransform->SetParent(GetTransform());
	TargetTransform->SetLocalPosition({ 0, 200, -1000 });

	CameraTransform = GetLevel()->GetMainCamera()->GetTransform();
}

void PlayerCamera::Update(float _DeltaTime)
{
	GetTransform()->AddWorldRotation(float4(0, GameEngineInput::GetMouseDirection().x, 0));

	//CameraTransform->SetWorldPosition(TargetTransform->GetWorldPosition());
	//CameraTransform->SetWorldRotation(TargetTransform->GetWorldRotation());
}

