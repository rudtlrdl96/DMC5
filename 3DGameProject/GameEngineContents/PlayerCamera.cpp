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

	CameraArm = GetLevel()->CreateActor<GameEngineActor>()->GetTransform();
	CameraArm->SetParent(GetTransform());

	CameraTarget = GetLevel()->CreateActor<GameEngineActor>()->GetTransform();
	CameraTarget->SetParent(CameraArm);
	CameraTarget->SetLocalPosition({ 0, 100, -350 });

	CameraTransform = GetLevel()->GetMainCamera()->GetTransform();
}

void PlayerCamera::Update(float _DeltaTime)
{
	if (GameEngineInput::IsFree("CLICK"))
	{
		GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * 5));
		CameraTransform->SetWorldPosition(CameraTarget->GetWorldPosition());
		return;
	}

	GetTransform()->AddWorldRotation({ 0, GameEngineInput::GetMouseDirection().x, 0 });

	float x = CameraArm->GetWorldRotation().x + GameEngineInput::GetMouseDirection().y;
	if (x < 180.0f)
	{
		x = std::clamp(x, -1.0f, 40.0f);
	}
	else
	{
		x = std::clamp(x, 10.0f, 21.0f);
	}
	CameraArm->SetLocalRotation({x, 0, 0});

	GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * 5));
	CameraTransform->SetWorldPosition(CameraTarget->GetWorldPosition());
	CameraTransform->SetWorldRotation(CameraTarget->GetWorldRotation());
}

