#pragma once
#include <GameEngineBase/GameEngineNetObject.h>

#include "ContentsEnum.h"
#include "PacketEnum.h"

class PhysXCapsuleComponent;

class NetworkObjectBase : public GameEngineNetObject
{
	friend class NetworkManager;

public:
	NetworkObjectBase();
	~NetworkObjectBase();

	NetworkObjectBase(const NetworkObjectBase& _Other) = delete;
	NetworkObjectBase(NetworkObjectBase&& _Other) noexcept = delete;
	NetworkObjectBase& operator=(const NetworkObjectBase& _Other) = delete;
	NetworkObjectBase& operator=(const NetworkObjectBase&& _Other) noexcept = delete;

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

	template <typename PacketType>
	void BindPacketFunction(PacketEnum _Type, std::function<void(std::shared_ptr<PacketType>)> _Callback = nullptr)
	{
		if (NetControllType::ActiveControll == GetControllType())
			return;

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

	template <typename DataType>
	void LinkData_UpdatePacket(DataType& Data)
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
		}
	}


private:
	static NetworkObjectBase* GetNetObj(unsigned int _ObjID);


	std::shared_ptr<PhysXCapsuleComponent> PhysXCapsule = nullptr;

	float4 Net_PrevPos = float4::ZERO;
	float4 Net_DestPos = float4::ZERO;
	//float4 Net_PrevRot = float4::ZERO;
	//float4 Net_DestRot = float4::ZERO;
	float Net_LerpTimer = 0.f;

	Net_ActorType NetActorType = Net_ActorType::UNKNOWN;

	float ActorTimeScale = 1.f;
	bool IsUpdateValue = true;
	std::vector<int*> UpdatePacket_IntLinkDatas;
	std::vector<bool*> UpdatePacket_BoolLinkDatas;
	std::vector<float*> UpdatePacket_FloatLinkDatas;
	
	
	std::map<PacketEnum, std::function<void(std::shared_ptr<GameEnginePacket> _Packet)>> PacketProcessFunctions;

	inline Net_ActorType GetNetObjectType() const
	{
		return NetActorType;
	}

	void SetNetwortTransData(const float4& _DestPos, const float4& _DestRot);

	void SetUpdateState(bool _IsOn);
};

