#pragma once
#include <GameEngineBase/GameEngineNetObject.h>

#include "ContentsEnum.h"

class PhysXCapsuleComponent;

class NetworkObjectBase : public GameEngineNetObject
{
public:
	NetworkObjectBase();
	~NetworkObjectBase();

	NetworkObjectBase(const NetworkObjectBase& _Other) = delete;
	NetworkObjectBase(NetworkObjectBase&& _Other) noexcept = delete;
	NetworkObjectBase& operator=(const NetworkObjectBase& _Other) = delete;
	NetworkObjectBase& operator=(const NetworkObjectBase&& _Other) noexcept = delete;

	inline std::shared_ptr<PhysXCapsuleComponent> GetPhysXComponent()
	{
		return PhysXCapsule;
	}

	inline void SetPhysXComponent(std::shared_ptr<PhysXCapsuleComponent> _Physics)
	{
		PhysXCapsule = _Physics;
	}




protected:
	inline void SetNetObjectType(Net_ActorType _ActorType)
	{
		GameEngineNetObject::SetNetObjectType(static_cast<int>(_ActorType));
	}

	void SetNetwortTransData(const float4& _DestPos, const float4& _DestRot);
	
	void Update_NetworkTrans(float _DeltaTime);

private:
	std::shared_ptr<PhysXCapsuleComponent> PhysXCapsule = nullptr;

	float4 Net_PrevPos = float4::ZERO;
	float4 Net_DestPos = float4::ZERO;
	//float4 Net_PrevRot = float4::ZERO;
	//float4 Net_DestRot = float4::ZERO;
	float Net_LerpTimer = 0.f;
};

