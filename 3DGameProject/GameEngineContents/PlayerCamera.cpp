#include "PrecompileHeader.h"
#include <GameEngineBase/GameEngineRandom.h>
#include "PlayerCamera.h"
PlayerCamera::PlayerCamera()
{
	SetName("PlayerCamera");
}

PlayerCamera::~PlayerCamera()
{
}

static bool IsShake = false;
static float ShakePower = 0.0f;
static float ShakeTimer = 0.0f;
static float ShakeTime = 0.0f;
static float ShakeFadeIn = 0.0f;
static float ShakeFadeOut = 0.0f;

void PlayerCamera::SetCameraCutscene(const float4& _StartPos, const float4& _EndPos, const float4& _StartRot, const float4& _EndRot, float _Time)
{
	CameraCutsceneStartPos = _StartPos;
	CameraCutsceneEndPos = _EndPos;
	CameraCutsceneStartRot = _StartRot;
	CameraCutsceneEndRot = _EndRot;
	CameraCutsceneSpeed = 1.0f / _Time;
	CameraCutsceneRatio = 0.0f;
	IsCameraCutscene = true;
}

void PlayerCamera::Shake(float _Power, float _Time, float _FadeIn, float _FadeOut)
{
	IsShake = true;
	ShakePower = _Power;
	ShakeTimer = 0;
	ShakeTime = _Time;
	ShakeFadeIn = _FadeIn;
	ShakeFadeOut = _FadeOut;
}

void PlayerCamera::SetRotation(const float4& _Rot)
{
	float4 YRot = GetTransform()->GetLocalRotation();
	YRot.y = _Rot.y;
	GetTransform()->SetLocalRotation(YRot);

	float4 XRot = CameraArm->GetLocalRotation();
	XRot.x = _Rot.x;
	CameraArm->SetLocalRotation(XRot);
}

void PlayerCamera::SetLookTarget(const float4& TargetPos)
{
	float4 Forward = GetTransform()->GetWorldForwardVector();
	Forward.y = 0;
	Forward.Normalize();
	float4 Dir = TargetPos - GetTransform()->GetWorldPosition();
	Dir.y = 0;
	Dir.Normalize();
	float Dot = float4::DotProduct3D(Forward, Dir);

	float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
	float4 Cross = float4::Cross3DReturnNormal(Forward, Dir);

	if (std::isnan(Angle))
	{
		return;
	}
	if (Cross.y < 0.0f)
	{
		GetTransform()->AddWorldRotation({ 0, -Angle, 0 });
	}
	else
	{
		GetTransform()->AddWorldRotation({ 0, Angle, 0 });
	}
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

	//GameEngineInput::ShowMouseCursor(false);
}

void PlayerCamera::Update(float _DeltaTime)
{
	if (true == IsCameraCutscene)
	{
		CutSceneUpdate(_DeltaTime);
		return;
	}

	if (true == FirstFrame)
	{
		GetTransform()->SetWorldPosition(PlayerTransform->GetWorldPosition());
		FirstFrame = false;
	}

	// ������ �� ���ο� ���� ���
	TargetCheck(_DeltaTime);
	CameraControll(_DeltaTime);
	float MulTrackingSpeed = std::clamp<float>((((GetTransform()->GetWorldPosition() - PlayerTransform->GetWorldPosition()).Size() - FastTrackingDistance) * FastTrackingSpeed), 1, 2);
	// ���� ���ο� ���� ī�޶� ���� �ӵ� ����
	if ((GetTransform()->GetWorldPosition() - PlayerTransform->GetWorldPosition()).Size() > 0.5f)
	{
		if (TargetTransform == nullptr)
		{
			GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * TrackingSpeed * MulTrackingSpeed));
		}
		else
		{
			GetTransform()->SetWorldPosition(float4::LerpClamp(GetTransform()->GetWorldPosition(), PlayerTransform->GetWorldPosition(), _DeltaTime * LockOnTrackingSpeed * MulTrackingSpeed));
		}
	}
	// ����ĳ��Ʈ�� �� ����
	WallCheck();

	// ī�޶� �̵�
	CameraTransform->SetWorldPosition(CameraTargetPos);
	CameraTransform->SetWorldRotation(CameraTarget->GetWorldRotation());

	ShakeUpdate(_DeltaTime);
}

void PlayerCamera::CameraControll(float _DeltaTime)
{
	if (GameEngineInput::IsDown("Escape"))
	{
		IsMouseControll = false;
		GameEngineInput::ShowMouseCursor(true);
	}
	if (1.0f < GameEngineInput::GetPressTime("EngineMouseLeft"))
	{
		IsMouseControll = true;
		GameEngineInput::ShowMouseCursor(false);
	}
	// ȸ�� �Է¹ޱ�
	// ���콺
	float4 MouseRot = GameEngineInput::GetMouseDirection();
	MouseRot = float4::Lerp(BeforeMouseRot, MouseRot, MouseAcceleration);
	BeforeMouseRot = MouseRot;

	// Ű����
	// Y ��
	if ((GameEngineInput::IsFree("Player_CameraLeft") && GameEngineInput::IsFree("Player_CameraRight")) ||
		(GameEngineInput::IsPress("Player_CameraLeft") && GameEngineInput::IsPress("Player_CameraRight")))
	{
		// �¿츦 �Ѵ� �����ų� �Ѵ� ���� ������
		CameraRot.y = std::lerp(CameraRot.y, 0.0f, CameraRotBraking);
	}
	else
	{
		if (GameEngineInput::IsPress("Player_CameraLeft"))
		{
			CameraRot.y -= CameraRotAcceleration * _DeltaTime;
		}
		if (GameEngineInput::IsPress("Player_CameraRight"))
		{
			CameraRot.y += CameraRotAcceleration * _DeltaTime;
		}
	}
	CameraRot.y = std::clamp(CameraRot.y, -1.0f, 1.0f);

	// X ��
	if ((GameEngineInput::IsFree("Player_CameraUp") && GameEngineInput::IsFree("Player_CameraDown")) ||
		(GameEngineInput::IsPress("Player_CameraUp") && GameEngineInput::IsPress("Player_CameraDown")))
	{
		// ���Ʒ��� �Ѵ� �����ų� �Ѵ� ���� ������
		CameraRot.x = std::lerp(CameraRot.x, 0.0f, CameraRotBraking);
	}
	else
	{
		if (GameEngineInput::IsPress("Player_CameraUp"))
		{
			CameraRot.x -= CameraRotAcceleration * _DeltaTime;
		}
		if (GameEngineInput::IsPress("Player_CameraDown"))
		{
			CameraRot.x += CameraRotAcceleration * _DeltaTime;
		}
	}
	CameraRot.x = std::clamp(CameraRot.x, -1.0f, 1.0f);


	// ȸ��
	GetTransform()->AddWorldRotation({ 0, CameraRot.y * CameraRotYSpeed * _DeltaTime });
	float4 ArmRot = CameraArm->GetLocalRotation();
	ArmRot.x += CameraRot.x * CameraRotXSpeed * _DeltaTime;

	if (true == IsMouseControll)
	{
		GetTransform()->AddWorldRotation({ 0, MouseRot.x * MouseSensitivity, 0 });
		ArmRot.x += MouseRot.y * MouseSensitivity;
	}
	ArmRot.x = std::clamp(ArmRot.x, -20.0f, 60.0f);
	CameraArm->SetLocalRotation(ArmRot);

	// �Ÿ�����
	if (GameEngineInput::IsPress("UI_UP"))
	{
		CameraDistance += float4::FORWARD * 100.0f * _DeltaTime;
	}
	if (GameEngineInput::IsPress("UI_Down"))
	{
		CameraDistance += float4::BACK * 100.0f * _DeltaTime;
	}
}

void PlayerCamera::TargetCheck(float _DeltaTime)
{
	if (BossTransform != nullptr)
	{
		float Distance = (GetTransform()->GetWorldPosition() - BossTransform->GetWorldPosition()).Size() * DistanceMul;
		if (MaxDistance < Distance)
		{
			CameraArm->SetLocalPosition(float4::LerpClamp(CameraArm->GetLocalPosition(), float4::ZERO, _DeltaTime * LockOnTrackingSpeed));
			CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), CameraDistance, _DeltaTime * LockOnTrackingSpeed));
			return;
		}
		Distance = std::max<float>(std::abs(Distance), MinDistance);
		CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), { 0, 100, -Distance }, _DeltaTime * LockOnTrackingSpeed));
		CameraArm->SetWorldPosition(float4::LerpClamp(CameraArm->GetWorldPosition(), float4::Lerp(GetTransform()->GetWorldPosition(), BossTransform->GetWorldPosition(), 0.3f), _DeltaTime * LockOnTrackingSpeed));
		return;
	}
	if (TargetTransform == nullptr) {
		// ���� ����� ���� ��
		// ������ ī�޶� ��ġ�� Lerp �̿��Ͽ� �̵�
		CameraArm->SetLocalPosition(float4::LerpClamp(CameraArm->GetLocalPosition(), float4::ZERO, _DeltaTime * LockOnTrackingSpeed));
		CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), CameraDistance, _DeltaTime * LockOnTrackingSpeed));
		return;
	}
	// ���� ����� ���� ��
	// ���� �÷��̾��� ���̸� �ٶ󺸸�. ���� �÷��̾��� �Ÿ���ŭ z���� �ڷ� �̵���Ų�� (z �̵��� �ּҰ��� ������ �ִ�)
	float Distance = (GetTransform()->GetWorldPosition() - TargetTransform->GetWorldPosition()).Size() * DistanceMul;
	Distance = std::max<float>(std::abs(Distance), MinDistance);
	if (MaxDistance < Distance)
	{
		CameraArm->SetLocalPosition(float4::LerpClamp(CameraArm->GetLocalPosition(), float4::ZERO, _DeltaTime * LockOnTrackingSpeed));
		CameraTarget->SetLocalPosition(float4::LerpClamp(CameraTarget->GetLocalPosition(), CameraDistance, _DeltaTime * LockOnTrackingSpeed));
		return;
	}
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

void PlayerCamera::ShakeUpdate(float _DeltaTime)
{
	if (false == IsShake) { return; }

	float Ratio = 1.0f;
	ShakeTimer += _DeltaTime;

	if (ShakeTime < ShakeTimer)
	{
		// �Էµ� ���� ���ӽð��� ���� ��� ����
		IsShake = false;
		return;
	}
	else if (ShakeTime - ShakeFadeOut < ShakeTimer)
	{
		// ���̵� �ƿ� �ð��� ��
		Ratio = (ShakeTime - ShakeTimer) / ShakeFadeOut;
	}
	else if (ShakeTimer < ShakeFadeIn)
	{
		// ���̵� �� �ð��� ��
		Ratio = ShakeTimer / ShakeFadeIn;
	}

	float x = GameEngineRandom::MainRandom.RandomFloat(-1, 1) * ShakePower * Ratio;
	float y = GameEngineRandom::MainRandom.RandomFloat(-1, 1) * ShakePower * Ratio;
	float z = GameEngineRandom::MainRandom.RandomFloat(-1, 1) * ShakePower * Ratio;

	CameraTransform->AddWorldPosition({ x, y, z });
}

void PlayerCamera::CutSceneUpdate(float _DeltaTime)
{
	CameraCutsceneRatio += CameraCutsceneSpeed * _DeltaTime;

	float4 Pos = float4::LerpClamp(CameraCutsceneStartPos, CameraCutsceneEndPos, CameraCutsceneRatio);
	float4 Rot = float4::LerpClamp(CameraCutsceneStartRot, CameraCutsceneEndRot, CameraCutsceneRatio);

	CameraTransform->SetWorldPosition(Pos);
	CameraTransform->SetWorldRotation(Rot);

	if (1 <= CameraCutsceneRatio)
	{
		IsCameraCutscene = false;
		CameraTransform->SetWorldPosition(CameraTargetPos);
		CameraTransform->SetWorldRotation(CameraTarget->GetWorldRotation());
	}
}

void PlayerCamera::DrawEditor()
{
	ImGui::InputFloat4("CameraDistance", CameraDistance.Arr1D);
	ImGui::InputFloat4("CameraFarDistance", CameraFarDistance.Arr1D);

	ImGui::InputFloat("MouseAcceleration", &MouseAcceleration);
	ImGui::InputFloat("MouseSensitivity", &MouseSensitivity);
	ImGui::InputFloat("CameraRotBraking", &CameraRotBraking);
	ImGui::InputFloat("CameraRotAcceleration", &CameraRotAcceleration);
	ImGui::InputFloat("CameraRotYSpeed", &CameraRotYSpeed);
	ImGui::InputFloat("CameraRotXSpeed", &CameraRotXSpeed);
	ImGui::InputFloat("DistanceMul", &DistanceMul);
	ImGui::InputFloat("MinDistance", &MinDistance);
	ImGui::InputFloat("MaxDistance", &MaxDistance);
	ImGui::InputFloat("TrackingSpeed", &TrackingSpeed);
	ImGui::InputFloat("LockOnTrackingSpeed", &LockOnTrackingSpeed);
	ImGui::InputFloat("FastTrackingDistance", &FastTrackingDistance);
	ImGui::InputFloat("FastTrackingSpeed", &FastTrackingSpeed);
}

