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
	CameraTarget->SetLocalPosition(CameraDistance);
	CameraTransform = GetLevel()->GetMainCamera()->GetTransform();
}

void PlayerCamera::Update(float _DeltaTime)
{
	// 락온한 적 여부에 따라 계산
	TargetCheck(_DeltaTime);

	// X축 회전은 제한값을 두기 위해 값을 받아둠.
	float x = CameraArm->GetWorldRotation().x;

	if (GameEngineInput::IsPress("CLICK"))
	{
		GetTransform()->AddWorldRotation({ 0, GameEngineInput::GetMouseDirection().x, 0 });
		x += GameEngineInput::GetMouseDirection().y;
	}
	if (GameEngineInput::IsPress("Player_CameraLeft"))
	{
		GetTransform()->AddWorldRotation({ 0, -CameraRotYSpeed * _DeltaTime, 0 });
	}
	if (GameEngineInput::IsPress("Player_CameraRight"))
	{
		GetTransform()->AddWorldRotation({ 0, CameraRotYSpeed * _DeltaTime, 0 });
	}
	if (GameEngineInput::IsPress("Player_CameraUp"))
	{
		x += -CameraRotXSpeed * _DeltaTime;
	}
	if (GameEngineInput::IsPress("Player_CameraDown"))
	{
		x += CameraRotXSpeed * _DeltaTime;
	}
	if (GameEngineInput::IsPress("UI_UP"))
	{
		CameraDistance += float4::FORWARD * 100.0f * _DeltaTime;
	}
	if (GameEngineInput::IsPress("UI_Down"))
	{
		CameraDistance += float4::BACK * 100.0f * _DeltaTime;
	}
	// X축 회전값 제한
	if (x < 180.0f)
	{
		x = std::clamp(x, -1.0f, 40.0f);
	}
	else
	{
		x = std::clamp(x, 10.0f, 21.0f);
	}
	CameraArm->SetLocalRotation({ x, 0, 0 });

	// 락온 여부에 따라 카메라 추적 속도 조절
	if (TargetTransform == nullptr)
	{
		GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * TrackingSpeed));
	}
	else
	{
		GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * LockOnTrackingSpeed));
	}

	// 레이캐스트로 벽 감지
	WallCheck();

	// 카메라 이동
	CameraTransform->SetWorldPosition(CameraTargetPos);
	CameraTransform->SetWorldRotation(CameraTarget->GetWorldRotation());
}

void PlayerCamera::TargetCheck(float _DeltaTime)
{
	if (TargetTransform == nullptr) {
		// 락온 대상이 없을 시
		// 기존의 카메라 위치로 Lerp 이용하여 이동
		CameraArm->SetLocalPosition(float4::LerpClamp(CameraArm->GetLocalPosition(), float4::ZERO, _DeltaTime * LockOnTrackingSpeed));

		CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), CameraDistance, _DeltaTime * LockOnTrackingSpeed));
		return;
	}
	// 락온 대상이 있을 시
	// 대상과 플레이어의 사이를 바라보며. 대상과 플레이어의 거리만큼 z값을 뒤로 이동시킨다 (z 이동은 최소값이 정해져 있다)
	float Distance = (GetTransform()->GetWorldPosition() - TargetTransform->GetWorldPosition()).Size() * 1.1f;
	Distance = std::max<float>(std::abs(Distance), MinDistance);

	CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), { 0, 100, -Distance }, _DeltaTime * LockOnTrackingSpeed));

	CameraArm->SetWorldPosition(float4::LerpClamp(CameraArm->GetWorldPosition(), (GetTransform()->GetWorldPosition() + TargetTransform->GetWorldPosition()) * 0.5f, _DeltaTime * LockOnTrackingSpeed));
}

void PlayerCamera::WallCheck()
{
	float4 playerpos = GetTransform()->GetWorldPosition();
	float4 playerposeye = CameraTarget->GetWorldPosition() - playerpos;
	float Distance = playerposeye.Size();
	float4 Colleye = float4::ZERO;

	if (true == GetLevel()->RayCast(playerpos, playerposeye, Colleye, Distance))
	{
		CameraTargetPos = Colleye;
		CameraTargetPos += CameraTarget->GetWorldForwardVector() * 50;
	}
	else
	{
		CameraTargetPos = CameraTarget->GetWorldPosition();
	}
}

