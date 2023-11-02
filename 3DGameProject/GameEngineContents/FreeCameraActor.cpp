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
	if (GameEngineInput::IsPress("UI_Left"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("UI_Right"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("UI_UP"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * _DeltaTime * MoveSpeed);
	}
	if (GameEngineInput::IsPress("UI_Down"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * _DeltaTime * MoveSpeed);
	}

	if (GameEngineInput::IsPress("CamMoveUp"))
	{
		MoveSpeed += 1000 * _DeltaTime;
	}
	if (GameEngineInput::IsPress("CamMoveDown"))
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
		GetTransform()->SetLocalPosition({ -500, 100, 0 });
		GetTransform()->SetLocalRotation({ 0, 90, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookRight"))
	{
		GetTransform()->SetLocalPosition({ 500, 100, 0 });
		GetTransform()->SetLocalRotation({ 0, -90, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookUp"))
	{
		GetTransform()->SetLocalPosition({ 0, 500, 0 });
		GetTransform()->SetLocalRotation({ 90, 0, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookDown"))
	{
		GetTransform()->SetLocalPosition({ 0, -500, 0 });
		GetTransform()->SetLocalRotation({ -90, 0, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookBack"))
	{
		GetTransform()->SetLocalPosition({ 0, 100, 500 });
		GetTransform()->SetLocalRotation({ 0, 180, 0 });
	}
	if (GameEngineInput::IsDown("FreeCam_LookFront"))
	{
		GetTransform()->SetLocalPosition({ 0, 100, -500 });
		GetTransform()->SetLocalRotation({ 0, 0, 0 });
	}

	CameraTransform->SetTransformData(GetTransform()->GetTransDataRef());

}

