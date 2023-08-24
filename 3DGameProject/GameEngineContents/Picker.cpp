#include "PrecompileHeader.h"
#include "Picker.h"
//#include "GlobalValues.h"
#include <GameEngineCore/GameEngineTransform.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "PickableActor.h"
//#include "AxisActor.h"
//#include "ActorAxis.h"


std::set<std::shared_ptr<PickableActor>> Picker::PickedActors;
std::shared_ptr<PickableActor> Picker::PickedActor;	// ��ŷ���� �浹�� �� �տ��ִ� ����
std::shared_ptr<PickableActor> Picker::ClickedActor;	// ��ŷ���� �浹X���� Ŭ������ �� ����
std::shared_ptr<PickableActor> Picker::SelectedActor;	// ��ŷ���� �浹X���� ����Ŭ�� ����

Picker::Picker()
{
}

Picker::~Picker()
{
}

void Picker::Start()
{
	//Collision_Ray_ = CreateComponent<GameEngineCollision>();
	//Collision_Ray_->GetTransform().SetWorldScale({ 0.1f, 0.1f, 200000 });
	//Collision_Ray_->ChangeOrder(CollisionGroup::Ray);
	//Collision_Ray_->SetDebugSetting(CollisionType::CT_OBB, float4(1.0f, 0, 0, 0.2f));

	//Axis_ = GetLevel()->CreateActor<ActorAxis>();
}

void Picker::Update(float _DeltaTime)
{
	// ī�޶�� ������ ��ġ 
	//float4 CamFwd = GetLevel()->GetMainCameraActor()->GetTransform().GetForwardVector();
	//CamPos_ = GetLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition() + CamFwd * 20.0f;
	//GetTransform().SetWorldPosition(CamPos_);


	// ȸ���� ����(���� ��)
	//float4 MouseVectorFromCam = GetLevel()->GetMainCamera()->GetMouseWorldPosition();
	//float4 PickerAngle = float4(MouseVectorFromCam.y * GameEngineMath::RadianToDegree * -1,
	//	MouseVectorFromCam.x * GameEngineMath::RadianToDegree,
	//	MouseVectorFromCam.z * GameEngineMath::RadianToDegree);

	// ȸ���� ����(���� ��)
	//float4 MouseVectorFromCam = GetLevel()->GetMainCamera()->GetMouseWorldPosition().Normalize3DReturn();

	//float XAngle = asinf(MouseVectorFromCam.x) * GameEngineMath::RadianToDegree;
	//float YAngle = asinf(MouseVectorFromCam.y) * GameEngineMath::RadianToDegree;

	//float4 PickerAngle = float4(YAngle * -1, XAngle, 0);

	//float4 Rot = GetLevel()->GetMainCameraActor()->GetTransform().GetLocalRotation();
	//GetTransform().SetWorldRotation(Rot + PickerAngle);

	// Ray�� �浹Ȯ��
	SelectPickedActor();

	// ��ŷ ������Ʈ Ŭ��üũ
	// ȭ�� ���̸� ��ŷX
	//float4 MouseScreenPos = GetLevel()->GetMainCamera()->GetMouseScreenPosition();
	//float ScaleX = GameEngineWindow::GetScale().x;
	//float ScaleY = GameEngineWindow::GetScale().y;
	//if (ScaleX < MouseScreenPos.x || ScaleY < MouseScreenPos.y)
	//{
	//	ClickedActor.reset();
	//	return;
	//}

	ClickCheck();

	ClickAxisControl();
}

// �������� ���͵��� ���� ������ ��
// ī�޶󺸴ٴ� �տ� �־����
void Picker::SelectPickedActor()
{
	//if (0 == PickedActors.size())
	//{
	//	PickedActor.reset();
	//	return;
	//}

	//std::shared_ptr<PickableActor> Nearest;
	//for (std::shared_ptr<PickableActor> Actor : PickedActors)
	//{
	//	if (nullptr == Nearest)
	//	{
	//		Nearest = Actor;
	//	}
	//	else
	//	{
	//		Nearest = Nearest->GetTransform().GetWorldPosition().z < Actor->GetTransform().GetWorldPosition().z ? Nearest : Actor;
	//	}
	//}
	//PickedActor = Nearest;
	//Nearest.reset();

}

void Picker::ClickCheck()
{
	// Ŭ������
	//if (true == GameEngineInput::GetInst()->IsDown("VK_LBUTTON"))
	//{
	//	if (nullptr == SelectedActor)
	//	{
	//		Axis_->Off();
	//	}

	//	// ����� Ŭ��
	//	if (nullptr == PickedActor)
	//	{
	//		SelectedActor.reset();
	//	}
	//	// ���͸� Ŭ���� -> ����/Axis ����
	//	else
	//	{
	//		ClickedActor = PickedActor;

	//		// AxisŬ��
	//		if (true == ClickedActor->IsAxis())
	//		{
	//			return;
	//		}
	//		// ���ο� ���� Ŭ��
	//		else
	//		{
	//			SelectedActor = ClickedActor;
	//			Axis_->On();
	//		}

	//	}

	//	return;
	//}

	//if (true == GameEngineInput::GetInst()->IsUp("VK_LBUTTON"))
	//{
	//	ClickedActor.reset();

	//	return;
	//}

}

void Picker::ClickAxisControl()
{
	//if (nullptr == ClickedActor)
	//{
	//	return;
	//}

	//float4 MouseDir = GetLevel()->GetMainCamera()->GetMouseWorldDir();

	//// �̵��ӵ�
	//float CamZ = ClickedActor->GetTransform().GetWorldPosition().z
	//	- GetLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition().z;

	//float4 MoveVec = float4::ZERO;
	//bool IsFront;

	//std::shared_ptr<PickableActor> ClickedAxis = std::dynamic_pointer_cast<PickableActor>(ClickedActor);
	//if (ClickedAxis->GetMoveDir().x > 0)
	//{
	//	IsFront = CheckFront(ClickedActor->GetTransform().GetForwardVector(),
	//		GetLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition().Normalize3DReturn());

	//	MoveVec.x = IsFront ? CamZ : CamZ;
	//	MoveVec *= MouseDir.x;
	//}
	//else if (ClickedAxis->GetMoveDir().y > 0)
	//{
	//	IsFront = CheckFront(ClickedActor->GetTransform().GetForwardVector(),
	//		GetLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition().Normalize3DReturn());

	//	MoveVec.y = IsFront ? CamZ * -1 : CamZ;
	//	MoveVec *= MouseDir.y;
	//}
	//else if (ClickedAxis->GetMoveDir().z > 0)
	//{
	//	float4 ActorPos = ClickedActor->GetTransform().GetWorldPosition();
	//	float4 CamPos = GetLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition();
	//	IsFront = CheckFront(ClickedActor->GetTransform().GetRightVector(),
	//		(ActorPos - CamPos).Normalize3DReturn());

	//	float Reverse = ActorPos.z - CamPos.z > 0 ? -1.0f : 1.0f;

	//	MoveVec.z = IsFront ? CamZ : CamZ * -1;
	//	MoveVec *= MouseDir.x * Reverse;
	//}

	//SelectedActor->GetTransform().SetWorldMove(MoveVec);
}

// ����ȭ�� ���� �ʿ�
bool Picker::CheckFront(float4 _Forward, float4 _Target)
{
	// ������ ������ ������ �Ǵ�(����)
	float Angle = acosf(float4::DotProduct3D(_Forward, _Target)) * GameEngineMath::RadToDeg;
	return Angle < 90 ? true : false;
}