#include "PrecompileHeader.h"
#include "NetworkObjectBase.h"

#include <GameEngineCore/GameEngineActor.h>

#include "NetworkGUI.h"

#include "NetworkManager.h"
#include "ObjectUpdatePacket.h"
#include "FsmChangePacket.h"
#include "NetEventPacket.h"


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


std::vector<NetworkObjectBase*> NetworkObjectBase::GetNetObjs(const std::vector<int>& _AllID)
{
	std::vector<GameEngineNetObject*> BaseNetObjects = GameEngineNetObject::GetNetObjects(_AllID);
	std::vector<NetworkObjectBase*> Result(BaseNetObjects.size(), nullptr);

	for (size_t i = 0; i < BaseNetObjects.size(); ++i)
	{
		GameEngineNetObject* BasePtr = BaseNetObjects[i];
		if (nullptr == BasePtr)
			continue;

		Result[i] = dynamic_cast<NetworkObjectBase*>(BasePtr);
	}

	return Result;
}


NetworkObjectBase::NetworkObjectBase()
{
	BindPacketCallBack<NetEventPacket>(PacketEnum::NetEventPacket, std::bind(&NetworkObjectBase::Process_NetObjEventPacket, this, std::placeholders::_1));
}

NetworkObjectBase::~NetworkObjectBase()
{

}






void NetworkObjectBase::Update_ProcessPacket()
{
	NetControllType ThisNetCtrlType = GetControllType();
	NetworkGUI* NetGUI = NetworkGUI::GetInst();

	//��Ŷ�� �� ó���� �� ����
	while (GameEngineNetObject::IsPacket())
	{
		//���� ó���� ��Ŷ�� Ÿ���� �˾ƿɴϴ�
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

		if (false == PacketProcessFunctions.contains(Type))
		{
			//�̰� �ݵ�� �Ͷ߷��� ��, �� �Ͷ߸��� ��Ŷ �׿��� �ٸ� ��Ŷ�� ó������ ����
			MsgAssert("�� ��Ŷ�� ó���ϱ� ���� �ݹ��Լ��� ��������� �ʾҽ��ϴ�.");
			return;
		}

		//��Ŷ ������
		std::shared_ptr<GameEnginePacket> Packet = PopFirstPacket();

		//Active���� ó���� ������� ���� ��Ŷ�̸鼭 �ڽ��� Active���� Ȯ��
		if (false == Packet->IsActiveRecv() && NetControllType::ActiveControll == ThisNetCtrlType)
		{
			//int PacketID = static_cast<int>(Packet->GetPacketID());
			//int ObjectID = static_cast<int>(Packet->GetObjectID());
			//std::string LogMsg = "PacketID : " + GameEngineString::ToString(PacketID) + " / ObjectID : " + GameEngineString::ToString(ObjectID);
			//
			//NetGUI->PrintLog("This is ActiveControll Objject", float4::RED);
			//NetGUI->PrintLog("So, This Packet can not process", float4::RED);
			//NetGUI->PrintLog(LogMsg.c_str(), float4::RED);
			MsgAssert("Active��Ʈ�ѿ��� ó���� ������� ���� ��Ŷ�ε�, Active��Ʈ�Ѱ�ü�� ��Ŷ�� ó���Ϸ��� �߽��ϴ�");
			continue;
		}

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


void NetworkObjectBase::SetFsmPacketCallBack(std::function<void(int _State)> _CallBack)
{
	BindPacketCallBack<FsmChangePacket>(PacketEnum::FsmChangePacket, [=](std::shared_ptr<FsmChangePacket> _Packet)
	{
		_CallBack(_Packet->FsmState);

		int AttackerID = _Packet->AttackerID;
		if (0 == AttackerID || nullptr == DamagedCallBack)
			return;

		NetworkObjectBase* AttacketPtr = nullptr;
		AttacketPtr = NetworkObjectBase::GetNetObj(AttackerID);
		if (nullptr == AttacketPtr)
		{
			MsgAssert("��Ŷ���� ���� ���ݴ���� nullptr�Դϴ�");
			return;
		}

		//�ݹ� ó��
		DamagedCallBack(AttacketPtr);

		//�ٽ� �ڽ��� �� FSM State�� ����Ǿ��ٴ� ���� �ٸ� ��Ʈ��ũ���� �˸�
		NetworkManager::SendFsmChangePacket(this, _Packet->FsmState);
	});
}


void NetworkObjectBase::BindNetObjEvent(int _EventType, const std::function<void(const std::vector<NetworkObjectBase*>&)>& _EventCallBack)
{
	if (true == NetworkEvents.contains(_EventType))
	{
		std::string Name = GetName().data();
		MsgAssert(Name  + "��ü�� " + GameEngineString::ToString(_EventType) + "�� ��ġ���� �̹� NetObjEvent�� �����մϴ�");
		return;
	}

	NetworkEvents[_EventType] = _EventCallBack;
}


void NetworkObjectBase::ExcuteNetObjEvent(int _EventType, NetObjEventPath _Path, const std::vector<NetworkObjectBase*>& _Targets /*= std::vector<NetworkObjectBase*>()*/)
{
	NetControllType CtrlType = GetControllType();

	if (NetObjEventPath::ActiveToPassive == _Path && NetControllType::PassiveControll == CtrlType)
		return;

	if (NetObjEventPath::PassiveToActive == _Path && NetControllType::ActiveControll == CtrlType)
		return;

	const std::function<void(std::vector<NetworkObjectBase*>)>& EventCallBack = NetworkEvents[_EventType];
	if (nullptr == EventCallBack)
	{
		MsgAssert("��ϵ��� ���� NetObjEvent�� ����Ϸ��� �߽��ϴ�");
		return;
	}


	std::vector<int> AllTargetID(_Targets.size(), 0);
	for (size_t i = 0; i < AllTargetID.size(); ++i)
	{
		if (nullptr == _Targets[i])
			continue;

		AllTargetID[i] = _Targets[i]->GetNetObjectID();
	}


	int NetObjID = GetNetObjectID();
	NetworkManager::PushNetworkEventPacket(_EventType, AllTargetID, NetObjID);

	std::string Name = GetName().data();
	NetworkGUI::GetInst()->PrintLog(Name + " Send NetObjEvent : " + GameEngineString::ToString(_EventType), float4::GREEN);
}

void NetworkObjectBase::Process_NetObjEventPacket(std::shared_ptr<class NetEventPacket> _Packet)
{
	int EventType = _Packet->GetEventType();
	NetworkGUI* NetGUI = NetworkGUI::GetInst();
	std::string Name = GetName().data();

	if (false == NetworkEvents.contains(EventType))
	{
		NetGUI->PrintLog(Name + " Faild " + GameEngineString::ToString(EventType) + "Network Object Event", float4::RED);
		return;
	}

	std::vector<int> AllTargetID = _Packet->GetAllTarget();
	std::vector<NetworkObjectBase*> AllTarget = GetNetObjs(AllTargetID);

	NetworkEvents[EventType](AllTarget);
	NetGUI->PrintLog(Name + " Success " + GameEngineString::ToString(EventType) + "Network Object Event", float4::GREEN);
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
	GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(this);
	if (nullptr == ActorPtr)
	{
		MsgAssert("Update��Ŷ�� �����Ҷ� ���ڷ� ���� ObjPtr�� GameEngineActor�� ��ӹ��� �ʾҽ��ϴ�");
		return;
	}

#pragma region ������Ʈ ��Ŷ ���� Ÿ�̹�
	/*
	1. �׳� On�϶� -> ���۵�
		OnOffValue : true -> true
		IsActorOn : true

	2. ó�� Off�϶� -> ���۵�
		OnOffValue : true -> false
		IsActorOn : false

	3. ��� Off �϶� -> ���۾ȵ�
		OnOffValue : false -> false
		IsActorOn : false

	3. �ٽ� On�϶� -> ���۵�
		OnOffValue : false -> true
		IsActorOn : true
	*/
#pragma endregion
	bool IsActorOn = ActorPtr->IsUpdate();
	if (false == OnOffValue && false == IsActorOn)
		return;

	OnOffValue = IsActorOn;

	NetworkManager::PushUpdatePacket
	(
		this, ActorPtr, ActorTimeScale,
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
