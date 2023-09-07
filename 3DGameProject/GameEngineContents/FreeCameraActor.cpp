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

	if (GameEngineInput::IsKey("SpeedUp") == false)
	{
		GameEngineInput::CreateKey("Click", MK_LBUTTON);
		GameEngineInput::CreateKey("SpeedUp", VK_UP);
		GameEngineInput::CreateKey("SpeedDown", VK_DOWN);

		GameEngineInput::CreateKey("LookLeft", VK_NUMPAD4);
		GameEngineInput::CreateKey("LookRight", VK_NUMPAD6);
		GameEngineInput::CreateKey("LookUp", VK_NUMPAD8);
		GameEngineInput::CreateKey("LookDown", VK_NUMPAD2);
		GameEngineInput::CreateKey("LookBack", VK_NUMPAD5);
		GameEngineInput::CreateKey("LookFront", VK_NUMPAD0);
	}
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

	if (GameEngineInput::IsPress("SpeedUp"))
	{
		MoveSpeed += 1000 * _DeltaTime;
	}
	if (GameEngineInput::IsPress("SpeedDown"))
	{
		MoveSpeed -= 1000 * _DeltaTime;
	}

	if (GameEngineInput::IsPress("Click"))
	{
		float4 MouseDir = GameEngineInput::GetMouseDirection() * 0.2f;
		GetTransform()->AddLocalRotation({ MouseDir.y, MouseDir.x, 0 });
	}

	if (GameEngineInput::IsDown("LookLeft"))
	{
		GetTransform()->SetLocalPosition({ -1000, 0, 0 });
		GetTransform()->SetLocalRotation({ 0, 90, 0 });
	}
	if (GameEngineInput::IsDown("LookRight"))
	{
		GetTransform()->SetLocalPosition({ 1000, 0, 0 });
		GetTransform()->SetLocalRotation({ 0, -90, 0 });
	}
	if (GameEngineInput::IsDown("LookUp"))
	{
		GetTransform()->SetLocalPosition({ 0, 1000, 0 });
		GetTransform()->SetLocalRotation({ 90, 0, 0 });
	}
	if (GameEngineInput::IsDown("LookDown"))
	{
		GetTransform()->SetLocalPosition({ 0, -1000, 0 });
		GetTransform()->SetLocalRotation({ -90, 0, 0 });
	}
	if (GameEngineInput::IsDown("LookBack"))
	{
		GetTransform()->SetLocalPosition({ 0, 0, 1000 });
		GetTransform()->SetLocalRotation({ 0, 180, 0 });
	}
	if (GameEngineInput::IsDown("LookFront"))
	{
		GetTransform()->SetLocalPosition({ 0, 0, -1000 });
		GetTransform()->SetLocalRotation({ 0, 0, 0 });
	}

	CameraTransform->SetTransformData(GetTransform()->GetTransDataRef());

}

