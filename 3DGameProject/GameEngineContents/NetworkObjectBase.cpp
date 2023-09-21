#include "PrecompileHeader.h"
#include "NetworkObjectBase.h"

#include "NetworkManager.h"


NetworkObjectBase* NetworkObjectBase::GetNetObj(unsigned int _ObjID)
{
	GameEngineNetObject* GameNetObj = nullptr;
	GameNetObj = GameEngineNetObject::GetNetObject(_ObjID);
	if (nullptr == GameNetObj)
		return nullptr;

	NetworkObjectBase* NetObj = nullptr;
	NetObj = dynamic_cast<NetworkObjectBase*>(GameNetObj);
	if (nullptr == NetObj)
	{
		MsgAssert("NetworkObjectBase�� �ƴ� GameEngineNetObject�� ��ӹ��� ��ü �߰�");
	}

	return NetObj;
}

NetworkObjectBase::NetworkObjectBase()
{

}

NetworkObjectBase::~NetworkObjectBase()
{

}

void NetworkObjectBase::SetNetwortTransData(const float4& _DestPos, const float4& _DestRot)
{
	if (NetControllType::ActiveControll == GetControllType())
	{
		MsgAssert("��Ŷ�� ���� �ʴ� NetControllType::UserControllŸ���� ���Ͱ� ObjectUpdatePacket�� �޾� ��ġ�� �����Ϸ��� �߽��ϴ�");
		return;
	}

	if (nullptr == PhysXCapsule)
	{
		MsgAssert("NetworkObjectBase�� Physicsĸ���� ��������� �ʾƼ� ��ġ/ȸ������ ������ �� �����ϴ�.");
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
	if (NetControllType::ActiveControll == GetControllType())
		return;

	if (nullptr == PhysXCapsule)
	{
		MsgAssert("NetworkObjectBase�� Physicsĸ���� ��������� �ʾƼ� ��ġ/ȸ������ ������ �� �����ϴ�.");
		return;
	}

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


void NetworkObjectBase::Update_ProcessPacket()
{
	//��Ŷ�� �� ó���� �� ����
	while (GameEngineNetObject::IsPacket())
	{
		//���� ó���� ��Ŷ�� Ÿ���� �˾ƿɴϴ�
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

		if (false == PacketProcessFunctions.contains(Type))
		{
			MsgAssert("�������ʿ��� �� ��Ŷ�� ó���ϱ� ���� �ݹ��Լ��� ��������� �ʾҰų� ��Ŷ ���� ����� ������� �ʾҽ��ϴ�");
			return;
		}

		std::shared_ptr<GameEnginePacket> Packet = PopFirstPacket();
		PacketProcessFunctions[Type](Packet);
	}
}

void NetworkObjectBase::Update_SendPacket(float _DeltaTime)
{
	NetworkManager::PushUpdatePacket
	(
		this, ActorTimeScale,
		UpdatePacket_IntLinkDatas,
		UpdatePacket_FloatLinkDatas,
		UpdatePacket_BoolLinkDatas
	);
}
