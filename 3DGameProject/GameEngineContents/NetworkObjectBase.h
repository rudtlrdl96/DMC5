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
	static void PushReservedDestroyCallback(Net_ActorType _Type, std::function<void()>&& _CallBack);
	static std::function<void()> PopReservedDestroyCallback(Net_ActorType _Type);

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

	//�� ���Ͱ� ������ �� ȣ��� �Լ��� ����մϴ�.
	inline void PushDestroyCallback(const std::function<void()>& _Callback)
	{
		DestroyCallbacks.emplace_back(_Callback);
	}

protected:
	//�̰� �ڽĿ��� ���Ƕ� Ǫ�ŵ� �ſ�. �ٵ� �� NetworkObjectBase::Destroy()�� ȣ������ּ���
	void Destroy() final;
	
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


	//������Ʈ ��Ŷ�� int/float/bool�� �߰��� �����մϴ�
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
			MsgAssert("������Ʈ ��Ŷ�� ������ ���� ���ø� Ÿ���Դϴ�");
			return;
		}

		if (nullptr == _DifferentCallBack)
			return;

		void* Ptr = &Data;
		if (true == LinkDifferentCallBacks.contains(Ptr))
		{
			MsgAssert("�ش� �����Ϳ��� �̹� DifferentCallBack�� �����մϴ�");
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
			MsgAssert("nullptr�� �Լ������͸� �־��ַ��� �߽��ϴ�");
			return;
		}

		DamagedCallBack = [_CallBack](NetworkObjectBase* _Attacker)
		{
			AttackerType* AttackerPtr = dynamic_cast<AttackerType*>(_Attacker);
			if (nullptr == AttackerPtr)
			{
				MsgAssert("�ǰ���Ŷ�� ó���ϴ� �������� �ٿ�ĳ���ÿ� �����Ͽ����ϴ�");
				return;
			}

			_CallBack(AttackerPtr);
		};
	}

private:
	static std::map<Net_ActorType, std::list<std::function<void()>>> ReservedDestroyCallbacks;

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
	
	std::vector<std::function<void()>> DestroyCallbacks;



	inline Net_ActorType GetNetObjectType() const
	{
		return NetActorType;
	}

	//NetworkManager���� UpdatePacket�� ������ �� ȣ���
	void SetNetwortTransData(const float4& _DestPos, const float4& _DestRot);

	//NetworkManager���� UpdatePacket�� ������ �� ȣ���
	void SetOnOffState(bool _IsOn);


	//NetworkManager���� UpdatePacket�� ������ �� ȣ���
	void SetUpdateArrData(std::shared_ptr<class ObjectUpdatePacket> _Packet);


	//ObejctUpdate��Ŷ�� ���Ź޾��� �� �迭 �����͵��� ���� ���� �� ������ ����ߴ� �ݹ� ȣ��
	template <typename DataType>
	void CopyUpdatePacketArrDatas(const std::vector<DataType*>& _ArrDest, const std::vector<DataType>& _ArrSource)
	{
		if (_ArrDest.size() != _ArrSource.size())
		{
			MsgAssert("�������� �������� ������Ʈ ��Ŷ Arr�����Ͱ� ���� �ʽ��ϴ�");
			return;
		}

		for (size_t i = 0; i < _ArrSource.size(); ++i)
		{
			DataType& Dest = (*_ArrDest[i]);
			const DataType& Source = _ArrSource[i];

			//float�϶� �Ǽ� �񱳰� ������� �׻� ����ɵ�
			if (Dest == Source)
				continue;

			//���� �� �����صΰ� �� �ٲٱ�
			DataType BeforeData = Source;
			Dest = Source;

			//�ݹ��Լ��� ������ ��쿡��
			void* DataPtr = &Dest;
			if (false == LinkDifferentCallBacks.contains(DataPtr))
				continue;

			//����, ������ ������ �־ �ݹ� ����
			LinkDifferentCallBacks[DataPtr](&BeforeData);
		}
	}


	//�ش� ��Ŷ�� ���Ź޾����� ó���� �ݹ��� ���
	template <typename PacketType>
	void BindPacketCallBack(PacketEnum _Type, std::function<void(std::shared_ptr<PacketType>)> _Callback = nullptr)
	{
		if (true == PacketProcessFunctions.contains(_Type))
		{
			MsgAssert("�ش� ��Ŷ�� ó�� �Լ��� �̹� ����Ͽ����ϴ�");
			return;
		}

		PacketProcessFunctions[_Type] = [=](std::shared_ptr<GameEnginePacket> _Packet)
		{
			//���ø��� �̿��� �ٿ�ĳ����
			std::shared_ptr<PacketType> ConvertPacket = std::dynamic_pointer_cast<PacketType>(_Packet);
			if (nullptr == ConvertPacket)
			{
				MsgAssert("��Ŷ Ÿ�� ��ȯ�� �����߽��ϴ�.");
			}

			if (nullptr == _Callback)
				return;

			_Callback(ConvertPacket);
		};
	}

	void SetActorTrans();
};

