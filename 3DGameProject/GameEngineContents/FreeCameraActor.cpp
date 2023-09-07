#include "PrecompileHeader.h"
#include "FreeCameraActor.h"

FreeCameraActor::FreeCameraActor()
{
	SetName("FreeCamera Actor");
}

FreeCameraActor::~FreeCameraActor()
{
}

void FreeCameraActor::Start()
{
	CameraTransform = GetLevel()->GetMainCamera()->GetTransform();

}

void FreeCameraActor::Update(float _DeltaTime)
{
	if (GameEngineInput::IsPress("CamMoveLeft"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("CamMoveRight"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("CamMoveForward"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("CamMoveBack"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("CamMoveUp"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("CamMoveDown"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * _DeltaTime * MoveSpeed);
	}

	if (GameEngineInput::IsPress("FreeCam_SpeedUp"))
	{
		MoveSpeed += 1000 * _DeltaTime;
	}
	if (GameEngineInput::IsPress("FreeCam_SpeedDown"))
	{
		MoveSpeed -= 1000 * _DeltaTime;
	}

	if (GameEngineInput::IsPress("EngineMouseLeft"))
	{
		float4 MouseDir = GameEngineInput::GetMouseDirection() * 0.2f;
		GetTransform()->AddLocalRotation({ MouseDir.y, MouseDir.x, 0 });
	}

	if (GameEngineInput::IsDown("FreeCam_LookLeft"))
	{
		GetTransform()->SetLocalPosition({ -1000, 0, 0 });
		GetTransform()->SetLocalRotation({ 0, 90, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookRight"))
	{
		GetTransform()->SetLocalPosition({ 1000, 0, 0 });
		GetTransform()->SetLocalRotation({ 0, -90, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookUp"))
	{
		GetTransform()->SetLocalPosition({ 0, 1000, 0 });
		GetTransform()->SetLocalRotation({ 90, 0, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookDown"))
	{
		GetTransform()->SetLocalPosition({ 0, -1000, 0 });
		GetTransform()->SetLocalRotation({ -90, 0, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookBack"))
	{
		GetTransform()->SetLocalPosition({ 0, 0, 1000 });
		GetTransform()->SetLocalRotation({ 0, 180, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookFront"))
	{
		GetTransform()->SetLocalPosition({ 0, 0, -1000 });
		GetTransform()->SetLocalRotation({ 0, 0, 0 });
	}

	CameraTransform->SetTransformData(GetTransform()->GetTransDataRef());

}

