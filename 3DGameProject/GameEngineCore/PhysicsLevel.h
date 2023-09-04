#pragma once

#include "PhysXDefault.h"
#include "GameEnginePhysics.h"

// Ό³Έν :
class PhysicsLevel
{
public:
	// constrcuter destructer
	PhysicsLevel();
	~PhysicsLevel();

	// delete Function
	PhysicsLevel(const PhysicsLevel& _Other) = delete;
	PhysicsLevel(PhysicsLevel&& _Other) noexcept = delete;
	PhysicsLevel& operator=(const PhysicsLevel& _Other) = delete;
	PhysicsLevel& operator=(PhysicsLevel&& _Other) noexcept = delete;

	inline physx::PxPhysics* GetLevelPhysics()
	{
		return GameEnginePhysics::GetPhysics();
	}

	inline physx::PxScene* GetLevelScene()
	{
		return GameEnginePhysics::GetCurrentScene();
	}

	inline physx::PxCooking* GetCooking()
	{
		return GameEnginePhysics::GetCooking();
	}

	void CreateScene(const std::string_view& _Name)
	{
		GameEnginePhysics::CreateScene(_Name);
	}

	void ChangeScene(std::shared_ptr<class GameEngineLevel> _Level)
	{
		GameEnginePhysics::ChangeScene(_Level);
	}

	void SetLevelSceneGravity(float _gravity)
	{
		GameEnginePhysics::SetSceneGravity(_gravity);
	}

	inline float GetLevelSceneGravity()
	{
		return GameEnginePhysics::GetSceneGravity();
	}

	bool RayCast(const float4& _vOrigin, const float4& _vDir, OUT float4& _vPoint, float _fDistance = 1000.f)
	{
		return GameEnginePhysics::RayCast(_vOrigin, _vDir, _vPoint, _fDistance);
	}

protected:

private:

};

