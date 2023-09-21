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

void NetworkObjectBase::SetNetwortTransData(const float4& _DestPos, const float4& _DestRot)
{
	if (NetControllType::ActiveControll == GetControllType())
	{
		MsgAssert("패킷을 받지 않는 NetControllType::UserControll타입의 엑터가 ObjectUpdatePacket을 받아 위치를 수정하려고 했습니다");
		return;
	}

	if (nullptr == PhysXCapsule)
	{
		MsgAssert("NetworkObjectBase에 Physics캡슐을 등록해주지 않아서 위치/회전값을 변경할 수 없습니다.");
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
	//서버로부터 패킷을 받는 타입의 객체만 아래를 실행
	if (NetControllType::ActiveControll == GetControllType())
		return;

	if (nullptr == PhysXCapsule)
	{
		MsgAssert("NetworkObjectBase에 Physics캡슐을 등록해주지 않아서 위치/회전값을 변경할 수 없습니다.");
		return;
	}

	//아래 코드들은 Lerp시키면서 부드럽게 이동/회전 시키는 코드
	Net_LerpTimer += _DeltaTime;
	float Ratio = (Net_LerpTimer / NetworkManager::PacketFlushTime);

	//Lerp이동
	float4 NowPos = float4::LerpClamp(Net_PrevPos, Net_DestPos, Ratio);
	PhysXCapsule->SetWorldPosition(NowPos);

	//Slerp회전
	//float4 NowRot = float4::SLerpQuaternion(Net_PrevRot, Net_DestRot, Ratio);
	//PhysXCapsule->SetWorldRotation(NowRot);
}