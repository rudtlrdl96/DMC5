#include "PrecompileHeader.h"
#include "PlayerCamera.h"

PlayerCamera::PlayerCamera() 
{
	SetName("PlayerCamera");
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
	CameraTarget->SetLocalPosition({ 0, 100, -300 });

	CameraTransform = GetLevel()->GetMainCamera()->GetTransform();
}

void PlayerCamera::Update(float _DeltaTime)
{
	float x = CameraArm->GetWorldRotation().x;

	if (GameEngineInput::IsPress("CLICK"))
	{
		GetTransform()->AddWorldRotation({ 0, GameEngineInput::GetMouseDirection().x, 0 });
		x += GameEngineInput::GetMouseDirection().y;
	}
	if (GameEngineInput::IsPress("Player_CameraLeft"))
	{
		GetTransform()->AddWorldRotation({ 0, -CameraRotSpeed * _DeltaTime, 0 });
	}
	if (GameEngineInput::IsPress("Player_CameraRight"))
	{
		GetTransform()->AddWorldRotation({ 0, CameraRotSpeed * _DeltaTime, 0 });
	}
	if (GameEngineInput::IsPress("Player_CameraUp"))
	{
		x += -CameraRotSpeed * _DeltaTime;
	}
	if (GameEngineInput::IsPress("Player_CameraDown"))
	{
		x += CameraRotSpeed * _DeltaTime;
	}

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

