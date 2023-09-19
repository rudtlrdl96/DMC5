#include "PrecompileHeader.h"
#include "NetworkObjectBase.h"

#include "NetworkManager.h"

NetworkObjectBase::NetworkObjectBase()
{

}

NetworkObjectBase::~NetworkObjectBase()
{

}

void NetworkObjectBase::SetNetwortTransData(const float4& _DestPos, const float4& _DestRot)
{
	if (NetControllType::UserControll == GetControllType())
	{
		MsgAssert("��Ŷ�� ���� �ʴ� NetControllType::UserControllŸ���� ���Ͱ� ObjectUpdatePacket�� �޾� ��ġ�� �����Ϸ��� �߽��ϴ�");
		return;
	}

	Net_PrevPos = PhysXCapsule->GetWorldPosition();
	Net_DestPos = _DestPos;
	//Net_PrevRot = PhysXCapsule->GetWorldRotation();
	//Net_DestRot = _DestRot;
	PhysXCapsule->SetWorldRotation(_DestRot);
	Net_LerpTimer = 0.f;
}


void NetworkObjectBase::Update_NetworkTrans(float _DeltaTime)
{
	//�����κ��� ��Ŷ�� �޴� Ÿ���� ��ü�� �Ʒ��� ����
	if (NetControllType::UserControll == GetControllType())
		return;

	//�Ʒ� �ڵ���� Lerp��Ű�鼭 �ε巴�� �̵�/ȸ�� ��Ű�� �ڵ�
	Net_LerpTimer += _DeltaTime;
	float Ratio = (Net_LerpTimer / NetworkManager::PacketFlushTime);

	//Lerp�̵�
	float4 NowPos = float4::LerpClamp(Net_PrevPos, Net_DestPos, Ratio);
	PhysXCapsule->SetWorldPosition(NowPos);

	//Slerpȸ��
	//float4 NowRot = float4::SLerpQuaternion(Net_PrevRot, Net_DestRot, Ratio);
	//PhysXCapsule->SetWorldRotation(NowRot);
}