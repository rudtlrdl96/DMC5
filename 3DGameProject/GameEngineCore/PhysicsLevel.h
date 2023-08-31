#pragma once

#include "PhysXDefault.h"
#include "GameEnginePhysics.h"

// ���� :
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

	void CreateScene(const std::string_view& _Name)
	{
		GameEnginePhysics::CreateScene(_Name);
	}

	void ChangeScene(const std::string_view& _Name)
	{
		GameEnginePhysics::ChangeScene(_Name);
	}

	void SetLevelSceneGravity(float _gravity)
	{
		GameEnginePhysics::SetSceneGravity(_gravity);
	}

	inline float GetLevelSceneGravity()
	{
		return GameEnginePhysics::GetSceneGravity();
	}

	inline physx::PxPhysics* GetLevelPhysics()
	{
		return GameEnginePhysics::GetPhysics();
	}

	inline physx::PxScene* GetLevelScene()
	{
		return GameEnginePhysics::GetCurrentScene();
	}

	bool RayCast(const float4& _vOrigin, const float4& _vDir, OUT float4& _vPoint, float _fDistance = 1000.f)
	{
		return GameEnginePhysics::RayCast(_vOrigin, _vDir, _vPoint, _fDistance);
	}

protected:

private:

};

