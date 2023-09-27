#include "PrecompileHeader.h"
#include "NetworkObjectBase.h"

#include "NetworkGUI.h"

#include "NetworkManager.h"
#include "ObjectUpdatePacket.h"
#include "FsmChangePacket.h"


NetworkObjectBase* NetworkObjectBase::DebugTarget = nullptr;
Net_ActorType NetworkObjectBase::DebugType = Net_ActorType::UNKNOWN;

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


void NetworkObjectBase::Update_ProcessPacket()
{
	//��Ŷ�� �� ó���� �� ����
	while (GameEngineNetObject::IsPacket())
	{
		//���� ó���� ��Ŷ�� Ÿ���� �˾ƿɴϴ�
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

		if (false == PacketProcessFunctions.contains(Type))
		{
			//MsgAssert("�� ��Ŷ�� ó���ϱ� ���� �ݹ��Լ��� ��������� �ʾҽ��ϴ�.");
			return;
		}

		std::shared_ptr<GameEnginePacket> Packet = PopFirstPacket();
		PacketProcessFunctions[Type](Packet);
	}
}



void NetworkObjectBase::SetNetwortTransData(const float4& _DestPos, const float4& _DestRot)
{
	if (NetControllType::ActiveControll == GetControllType())
	{
		MsgAssert("��Ŷ�� ���� �ʴ� NetControllType::UserControllŸ���� ���Ͱ� ObjectUpdatePacket�� �޾� ��ġ�� �����Ϸ��� �߽��ϴ�");
		return;
	}

	//Physics�� ������� �ʴ� ��ü�� ��
	if (nullptr == PhysXCapsule)
	{
		SetActorTrans();
		
		Net_PrevPos = ActorTrans->GetWorldPosition();
		Net_DestPos = _DestPos;
		ActorTrans->SetWorldRotation(_DestRot);
		Net_LerpTimer = 0.f;
	}

	//Physics�� ����ϴ� ��ü�� ��
	else
	{
		Net_PrevPos = PhysXCapsule->GetWorldPosition();
		Net_DestPos = _DestPos;
		PhysXCapsule->SetWorldRotation(_DestRot);
		Net_LerpTimer = 0.f;
	}

	if (DebugTarget != this)
		return;

	const std::string& PrevPos = Net_PrevPos.ConvertString();
	const std::string& NextPos = Net_DestPos.ConvertString();

	NetworkGUI::GetInst()->PrintLog("[UpdatePacket Setting PrevPos]", float4{ 0.f, 1.f, 1.f, 1.f });
	NetworkGUI::GetInst()->PrintLog(PrevPos);
	NetworkGUI::GetInst()->PrintLog("[UpdatePacket Setting NextPos]", float4{ 0.f, 1.f, 1.f, 1.f });
	NetworkGUI::GetInst()->PrintLog(NextPos);
}

void NetworkObjectBase::SetUpdateArrData(std::shared_ptr<ObjectUpdatePacket> _Packet)
{
	const std::vector<int>& IntDatas = _Packet->GetIntDatas();
	const std::vector<float>& FloatDatas = _Packet->GetFloatDatas();
	const std::vector<bool>& BoolDatas = _Packet->GetBoolDatas();
	
	CopyUpdatePacketArrDatas<int>(UpdatePacket_IntLinkDatas, IntDatas);
	CopyUpdatePacketArrDatas<float>(UpdatePacket_FloatLinkDatas, FloatDatas);
	CopyUpdatePacketArrDatas<bool>(UpdatePacket_BoolLinkDatas, BoolDatas);
}


void NetworkObjectBase::LinkChild_UpdatePacket(GameEngineTransform* _Child)
{
	float4 Rotation = _Child->GetLocalRotation();
	float4 Position = _Child->GetLocalPosition();
	
	ChildData& LinkData = Children[_Child];
	LinkData.Rotation = Rotation;
	LinkData.Position = Position;

	for (size_t i = 0; i < 3; ++i)
	{
		float& LinkDataRot = LinkData.Rotation.Arr1D[i];
		float& LinkDataPos = LinkData.Position.Arr1D[i];

		LinkData_UpdatePacket<float>(LinkDataRot);
		LinkData_UpdatePacket<float>(LinkDataPos);
	}

	BindPacketCallBack<ObjectUpdatePacket>(PacketEnum::ObjectUpdatePacket, [this](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		for (const std::pair<GameEngineTransform*, ChildData>& Pair : Children)
		{
			GameEngineTransform* Child = Pair.first;
			const ChildData& Data = Pair.second;
			
			Child->SetLocalRotation(Data.Rotation);
			Child->SetLocalPosition(Data.Position);
		}
	});
}


void NetworkObjectBase::SetFsmPacketCallBack(std::function<void(int _State)> _CallBack)
{
	BindPacketCallBack<FsmChangePacket>(PacketEnum::FsmChangePacket, [=](std::shared_ptr<FsmChangePacket> _Packet)
	{
		_CallBack(_Packet->FsmState);
	});
}



void NetworkObjectBase::Update_NetworkTrans(float _DeltaTime)
{
	//�����κ��� ��Ŷ�� �޴� Ÿ���� ��ü�� �Ʒ��� ����
	if (NetControllType::ActiveControll == GetControllType())
		return;

	//�Ʒ� �ڵ���� Lerp��Ű�鼭 �ε巴�� �̵�/ȸ�� ��Ű�� �ڵ�(Lerp�̵�)
	Net_LerpTimer += _DeltaTime;
	float Ratio = (Net_LerpTimer / NetworkManager::PacketFlushTime);
	float4 NowPos = float4::LerpClamp(Net_PrevPos, Net_DestPos, Ratio);

	if (nullptr == PhysXCapsule)
	{
		SetActorTrans();
		ActorTrans->SetWorldPosition(NowPos);
		return;
	}

	PhysXCapsule->SetWorldPosition(NowPos);
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


void NetworkObjectBase::SetOnOffState(bool _IsOn)
{
	if (_IsOn == OnOffValue)
		return;

	GameEngineActor* ActorPtr = nullptr;
	ActorPtr  = dynamic_cast<GameEngineActor*>(this);
	if (nullptr == ActorPtr)
	{
		MsgAssert("NetworkObjectBase�� GameEngineActor�� ĳ�����ϴµ� �����Ͽ����ϴ�.");
		return;
	}

	OnOffValue = _IsOn;
	if (true == OnOffValue)
	{
		ActorPtr->On();
	}
	else
	{
		ActorPtr->Off();
	}
}



void NetworkObjectBase::SetActorTrans()
{
	if (nullptr != ActorTrans)
		return;

	GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(this);
	if (nullptr == ActorPtr)
	{
		MsgAssert("NetworkObjectBase�� GameEngineActor�� ��ȯ�ϴµ� �����Ͽ����ϴ�");
		return;
	}

	if (true == ActorPtr->IsPhysicsActor())
	{
		MsgAssert("������ UpdatePacket�� ó���� �� Physics�� ����ϴ� ��ü�� Transform�� �̿��Ͽ� ó���Ϸ��� �߽��ϴ�");
		return;
	}

	ActorTrans = ActorPtr->GetTransform();
}

