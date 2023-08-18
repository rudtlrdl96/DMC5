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

	CameraTransform = GetLevel()->GetMainCamera()->GetTransform();
	//CameraTransform->SetParent(GetTransform());
	CameraTransform->SetLocalPosition({ 0, 0, 0 });
}

void PlayerCamera::Update(float _DeltaTime)
{
}

