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
	// ������ �� ���ο� ���� ���
	TargetCheck(_DeltaTime);

	// X�� ȸ���� ���Ѱ��� �α� ���� ���� �޾Ƶ�.
	float x = CameraArm->GetWorldRotation().x;

	if (GameEngineInput::IsPress("EngineMouseLeft"))
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

	x = std::clamp(x, -20.0f, 60.0f);

	//// X�� ȸ���� ����
	//if (x < 180.0f)
	//{
	//	x = std::clamp(x, -1.0f, 40.0f);
	//}
	//else
	//{
	//	x = std::clamp(x, 10.0f, 21.0f);
	//}
	CameraArm->SetLocalRotation({ x, 0, 0 });

	// ���� ���ο� ���� ī�޶� ���� �ӵ� ����
	if (TargetTransform == nullptr)
	{
		GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * TrackingSpeed));
	}
	else
	{
		GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * LockOnTrackingSpeed));
	}

	// ����ĳ��Ʈ�� �� ����
	WallCheck();

	// ī�޶� �̵�
	CameraTransform->SetWorldPosition(CameraTargetPos);
	CameraTransform->SetWorldRotation(CameraTarget->GetWorldRotation());
}

void PlayerCamera::TargetCheck(float _DeltaTime)
{
	if (TargetTransform == nullptr) {
		// ���� ����� ���� ��
		// ������ ī�޶� ��ġ�� Lerp �̿��Ͽ� �̵�
		CameraArm->SetLocalPosition(float4::LerpClamp(CameraArm->GetLocalPosition(), float4::ZERO, _DeltaTime * LockOnTrackingSpeed));
		CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), CameraDistance, _DeltaTime * LockOnTrackingSpeed));
		return;
	}
	// ���� ����� ���� ��
	// ���� �÷��̾��� ���̸� �ٶ󺸸�. ���� �÷��̾��� �Ÿ���ŭ z���� �ڷ� �̵���Ų�� (z �̵��� �ּҰ��� ������ �ִ�)
	float Distance = (GetTransform()->GetWorldPosition() - TargetTransform->GetWorldPosition()).Size() * 1.1f;
	Distance = std::max<float>(std::abs(Distance), MinDistance);

	CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), { 0, 100, -Distance }, _DeltaTime * LockOnTrackingSpeed));

	CameraArm->SetWorldPosition(float4::LerpClamp(CameraArm->GetWorldPosition(), (GetTransform()->GetWorldPosition() + TargetTransform->GetWorldPosition()) * 0.5f, _DeltaTime * LockOnTrackingSpeed));
}

void PlayerCamera::WallCheck()
{
	float4 playerpos = GetTransform()->GetWorldPosition();
	float4 playerposeye = CameraTarget->GetWorldPosition() - playerpos;
	if (playerposeye == float4::ZERO) { return; }
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

