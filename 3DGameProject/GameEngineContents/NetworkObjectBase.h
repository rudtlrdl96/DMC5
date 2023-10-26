#pragma once
#include <GameEngineBase/GameEngineNetObject.h>

#include "ContentsEnum.h"
#include "PacketEnum.h"

class PhysXCapsuleComponent;
class GameEngineTransform;

class NetworkObjectBase : public GameEngineNetObject, public GameEngineActor
{
	friend class NetworkManager;

public:
	static inline NetworkObjectBase* GetDebugTarget()
	{
		return DebugTarget;
	}

	static inline void SetDebugTarget(NetworkObjectBase* _Target)
	{
		DebugTarget = _Target;
	}

	static Net_ActorType GetDebugType()
	{
		return DebugType;
	}

	NetworkObjectBase();
	~NetworkObjectBase();

	NetworkObjectBase(const NetworkObjectBase& _Other) = delete;
	NetworkObjectBase(NetworkObjectBase&& _Other) noexcept = delete;
	NetworkObjectBase& operator=(const NetworkObjectBase& _Other) = delete;
	NetworkObjectBase& operator=(const NetworkObjectBase&& _Other) noexcept = delete;

	inline void SetTimeScale(float _Value)
	{
		ActorTimeScale = _Value;
	}

	inline float GetTimeScale() const
	{
		return ActorTimeScale;
	}

protected:
	inline void SetNetObjectType(Net_ActorType _ActorType)
	{
		NetActorType = _ActorType;
	}

	inline void BindPhysicsWithNet(std::shared_ptr<PhysXCapsuleComponent> _Physics)
	{
		PhysXCapsule = _Physics;
	}

	void Update_NetworkTrans(float _DeltaTime);

	void Update_ProcessPacket() final;
	void Update_SendPacket(float _DeltaTime) final;


	//업데이트 패킷의 int/float/bool를 추가로 연결합니다
	template <typename DataType>
	void LinkData_UpdatePacket(DataType& Data, std::function<void(DataType _BeforeData)> _DifferentCallBack = nullptr)
	{
		if constexpr (std::is_same<DataType, int>::value)
		{
			UpdatePacket_IntLinkDatas.push_back(&Data);
		}
		else if constexpr (std::is_same<DataType, float>::value)
		{
			UpdatePacket_FloatLinkDatas.push_back(&Data);
		}
		else if constexpr (std::is_same<DataType, bool>::value)
		{
			UpdatePacket_BoolLinkDatas.push_back(&Data);
		}
		else
		{
			MsgAssert("업데이트 패킷에 넣을수 없는 템플릿 타입입니다");
			return;
		}

		if (nullptr == _DifferentCallBack)
			return;

		void* Ptr = &Data;
		if (true == LinkDifferentCallBacks.contains(Ptr))
		{
			MsgAssert("해당 데이터에는 이미 DifferentCallBack이 존재합니다");
			return;
		}
		
		LinkDifferentCallBacks[Ptr] = [=](void* _BeforeData)
		{
			DataType* BeforePtr = reinterpret_cast<DataType*>(_BeforeData);
			_DifferentCallBack(*BeforePtr);
		};
	}

	void SetFsmPacketCallBack(std::function<void(int _State)> _CallBack);

	
	template <typename AttackerType>
	void SetDamagedNetCallBack(std::function<void(AttackerType* _Attacker)> _CallBack)
	{
		if (nullptr == _CallBack)
		{
			MsgAssert("nullptr인 함수포인터를 넣어주려고 했습니다");
			return;
		}

		DamagedCallBack = [_CallBack](NetworkObjectBase* _Attacker)
		{
			AttackerType* AttackerPtr = dynamic_cast<AttackerType*>(_Attacker);
			if (nullptr == AttackerPtr)
			{
				MsgAssert("피격패킷을 처리하는 과정에서 다운캐스팅에 실패하였습니다");
				return;
			}

			_CallBack(AttackerPtr);
		};
	}

private:
	static NetworkObjectBase* DebugTarget;
	static Net_ActorType DebugType;

	static NetworkObjectBase* GetNetObj(unsigned int _ObjID);

	std::function<void(NetworkObjectBase*)> DamagedCallBack = nullptr;
	std::shared_ptr<PhysXCapsuleComponent> PhysXCapsule = nullptr;
	class GameEngineTransform* ActorTrans = nullptr;

	float4 Net_PrevPos = float4::ZERO;
	float4 Net_DestPos = float4::ZERO;
	float Net_LerpTimer = 0.f;

	Net_ActorType NetActorType = Net_ActorType::UNKNOWN;

	float ActorTimeScale = 1.f;
	bool OnOffValue = true;
	std::vector<int*> UpdatePacket_IntLinkDatas;
	std::vector<bool*> UpdatePacket_BoolLinkDatas;
	std::vector<float*> UpdatePacket_FloatLinkDatas;

	std::map<void*, std::function<void(void* _BeforeData)>> LinkDifferentCallBacks;
	std::map<PacketEnum, std::function<void(std::shared_ptr<GameEnginePacket> _Packet)>> PacketProcessFunctions;
	
	inline Net_ActorType GetNetObjectType() const
	{
		return NetActorType;
	}

	//NetworkManager에서 UpdatePacket을 수신할 때 호출됨
	void SetNetwortTransData(const float4& _DestPos, const float4& _DestRot);

	//NetworkManager에서 UpdatePacket을 수신할 때 호출됨
	void SetOnOffState(bool _IsOn);


	//NetworkManager에서 UpdatePacket을 수신할 때 호출됨
	void SetUpdateArrData(std::shared_ptr<class ObjectUpdatePacket> _Packet);


	//ObejctUpdate패킷을 수신받았을 때 배열 데이터들의 값을 수정 및 사전에 등록했던 콜백 호출
	template <typename DataType>
	void CopyUpdatePacketArrDatas(const std::vector<DataType*>& _ArrDest, const std::vector<DataType>& _ArrSource)
	{
		if (_ArrDest.size() != _ArrSource.size())
		{
			MsgAssert("전송측과 수신측의 업데이트 패킷 Arr데이터가 맞지 않습니다");
			return;
		}

		for (size_t i = 0; i < _ArrSource.size(); ++i)
		{
			DataType& Dest = (*_ArrDest[i]);
			const DataType& Source = _ArrSource[i];

			//float일땐 실수 비교가 어려워서 항상 통과될듯
			if (Dest == Source)
				continue;

			//이전 값 저장해두고 값 바꾸기
			DataType BeforeData = Source;
			Dest = Source;

			//콜백함수가 존재한 경우에만
			void* DataPtr = &Dest;
			if (false == LinkDifferentCallBacks.contains(DataPtr))
				continue;

			//이전, 이후의 값으로 넣어서 콜백 실행
			LinkDifferentCallBacks[DataPtr](&BeforeData);
		}
	}


	//해당 패킷을 수신받았을때 처리할 콜백을 등록
	template <typename PacketType>
	void BindPacketCallBack(PacketEnum _Type, std::function<void(std::shared_ptr<PacketType>)> _Callback = nullptr)
	{
		if (true == PacketProcessFunctions.contains(_Type))
		{
			MsgAssert("해당 패킷의 처리 함수는 이미 등록하였습니다");
			return;
		}

		PacketProcessFunctions[_Type] = [=](std::shared_ptr<GameEnginePacket> _Packet)
		{
			//템플릿을 이용해 다운캐스팅
			std::shared_ptr<PacketType> ConvertPacket = std::dynamic_pointer_cast<PacketType>(_Packet);
			if (nullptr == ConvertPacket)
			{
				MsgAssert("패킷 타입 변환에 실패했습니다.");
			}

			if (nullptr == _Callback)
				return;

			_Callback(ConvertPacket);
		};
	}

	void SetActorTrans();
};

