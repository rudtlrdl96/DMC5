#include "PrecompileHeader.h"
#include "NetworkObjectBase.h"

#include <GameEngineCore/GameEngineActor.h>

#include "NetworkGUI.h"

#include "NetworkManager.h"
#include "ObjectUpdatePacket.h"
#include "FsmChangePacket.h"


NetworkObjectBase* NetworkObjectBase::DebugTarget = nullptr;
Net_ActorType NetworkObjectBase::DebugType = Net_ActorType::UNKNOWN;

std::map<Net_ActorType, std::list<std::function<void()>>> NetworkObjectBase::ReservedDestroyCallbacks;

void NetworkObjectBase::PushReservedDestroyCallback(Net_ActorType _Type, const std::function<void()>& _CallBack)
{
	if (nullptr == _CallBack)
	{
		MsgAssert("인자로 들어온 콜백이 nullptr일 수는 없습니다");
		return;
	}

	ReservedDestroyCallbacks[_Type].push_back(_CallBack);

	int Size = static_cast<int>(ReservedDestroyCallbacks[_Type].size());
	NetworkGUI::GetInst()->PrintLog("[Push] CallBack List Size : " + GameEngineString::ToString(Size));
}

std::function<void()> NetworkObjectBase::PopReservedDestroyCallback(Net_ActorType _Type)
{
	std::list<std::function<void()>>& Group = ReservedDestroyCallbacks[_Type];
	if (true == Group.empty())
		return nullptr;

	const std::function<void()>& Return = Group.front();
	Group.pop_front();

	int Size = static_cast<int>(ReservedDestroyCallbacks[_Type].size());
	NetworkGUI::GetInst()->PrintLog("[Pop] CallBack List Size : " + GameEngineString::ToString(Size));

	return Return;
}

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
		MsgAssert("NetworkObjectBase가 아닌 GameEngineNetObject를 상속받은 객체 발견");
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
	NetControllType ThisNetCtrlType = GetControllType();
	NetworkGUI* NetGUI = NetworkGUI::GetInst();

	//패킷을 다 처리할 때 까지
	while (GameEngineNetObject::IsPacket())
	{
		//지금 처리할 패킷의 타입을 알아옵니다
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

		if (false == PacketProcessFunctions.contains(Type))
		{
			//이거 반드시 터뜨려야 함, 안 터뜨리면 패킷 쌓여서 다른 패킷이 처리되지 않음
			MsgAssert("이 패킷을 처리하기 위한 콜백함수를 등록해주지 않았습니다.");
			return;
		}

		//패킷 꺼내기
		std::shared_ptr<GameEnginePacket> Packet = PopFirstPacket();

		//Active에서 처리가 허락되지 않은 패킷이면서 자신이 Active인지 확인
		if (false == Packet->IsActiveRecv() && NetControllType::ActiveControll == ThisNetCtrlType)
		{
			//int PacketID = static_cast<int>(Packet->GetPacketID());
			//int ObjectID = static_cast<int>(Packet->GetObjectID());
			//std::string LogMsg = "PacketID : " + GameEngineString::ToString(PacketID) + " / ObjectID : " + GameEngineString::ToString(ObjectID);
			//
			//NetGUI->PrintLog("This is ActiveControll Objject", float4::RED);
			//NetGUI->PrintLog("So, This Packet can not process", float4::RED);
			//NetGUI->PrintLog(LogMsg.c_str(), float4::RED);
			MsgAssert("Active컨트롤에서 처리가 허락되지 않은 패킷인데, Active컨트롤객체가 패킷을 처리하려고 했습니다");
			continue;
		}

		PacketProcessFunctions[Type](Packet);
	}
}



void NetworkObjectBase::SetNetwortTransData(const float4& _DestPos, const float4& _DestRot)
{
	if (NetControllType::ActiveControll == GetControllType())
	{
		MsgAssert("패킷을 받지 않는 NetControllType::UserControll타입의 엑터가 ObjectUpdatePacket을 받아 위치를 수정하려고 했습니다");
		return;
	}

	//Physics를 사용하지 않는 물체일 때
	if (nullptr == PhysXCapsule)
	{
		SetActorTrans();
		
		Net_PrevPos = ActorTrans->GetWorldPosition();
		Net_DestPos = _DestPos;
		ActorTrans->SetWorldRotation(_DestRot);
		Net_LerpTimer = 0.f;
	}

	//Physics를 사용하는 물체일 때
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
			MsgAssert("패킷으로 받은 공격대상이 nullptr입니다");
			return;
		}

		//콜백 처리
		DamagedCallBack(AttacketPtr);

		//다시 자신이 이 FSM State로 변경되었다는 것을 다른 네트워크에게 알림
		NetworkManager::SendFsmChangePacket(this, _Packet->FsmState);
	});
}



void NetworkObjectBase::Update_NetworkTrans(float _DeltaTime)
{
	//서버로부터 패킷을 받는 타입의 객체만 아래를 실행
	if (NetControllType::ActiveControll == GetControllType())
		return;

	//아래 코드들은 Lerp시키면서 부드럽게 이동/회전 시키는 코드(Lerp이동)
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
		MsgAssert("Update패킷을 전송할때 인자로 받은 ObjPtr이 GameEngineActor를 상속받지 않았습니다");
		return;
	}

#pragma region 업데이트 패킷 전송 타이밍
	/*
	1. 그냥 On일때 -> 전송됨
		OnOffValue : true -> true
		IsActorOn : true

	2. 처음 Off일때 -> 전송됨
		OnOffValue : true -> false
		IsActorOn : false

	3. 계속 Off 일때 -> 전송안됨
		OnOffValue : false -> false
		IsActorOn : false

	3. 다시 On일때 -> 전송됨
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
		MsgAssert("NetworkObjectBase를 GameEngineActor로 캐스팅하는데 실패하였습니다.");
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
		MsgAssert("NetworkObjectBase를 GameEngineActor로 변환하는데 실패하였습니다");
		return;
	}

	if (true == ActorPtr->IsPhysicsActor())
	{
		MsgAssert("서버의 UpdatePacket을 처리할 때 Physics를 사용하는 물체를 Transform을 이용하여 처리하려고 했습니다");
		return;
	}

	ActorTrans = ActorPtr->GetTransform();
}

void NetworkObjectBase::Destroy()
{
	GameEngineActor::Destroy();

	for (const std::function<void()>& CallBack : DestroyCallbacks)
	{
		if (nullptr == CallBack)
			continue;

		CallBack();
	}

	DestroyCallbacks.clear();
}

void NetworkObjectBase::PushDestroyCallback(const std::function<void()>& _Callback)
{
	DestroyCallbacks.push_back(_Callback);

	int Size = static_cast<int>(DestroyCallbacks.size());
	std::string Name = GetName().data();

	NetworkGUI::GetInst()->PrintLog(Name + " : " + GameEngineString::ToString(Size));
}