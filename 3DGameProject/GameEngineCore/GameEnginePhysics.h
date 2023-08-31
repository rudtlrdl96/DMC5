#pragma once

#include "PhysXDefault.h"
#include "ThirdParty/PhysX_4_1/inc/PhysX/foundation/PxErrorCallback.h"
#include "CustomCallback.h"

class UserErrorCallback : public physx::PxErrorCallback
{
private:
	void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
	{
		std::string Code = "Code : " + std::to_string(code)
			+ "\nMsg : " + message
			+ "\nFile : " + file
			+ "\nLine : " + std::to_string(line);
		MsgAssert(Code);
	}
};

// Ό³Έν :
class GameEnginePhysics
{
	friend class PhysicsLevel;
	friend class GameEngineLevel;
	friend class GameEngineCore;

public:
	// constrcuter destructer
	GameEnginePhysics();
	~GameEnginePhysics();

	// delete Function
	GameEnginePhysics(const GameEnginePhysics& _Other) = delete;
	GameEnginePhysics(GameEnginePhysics&& _Other) noexcept = delete;
	GameEnginePhysics& operator=(const GameEnginePhysics& _Other) = delete;
	GameEnginePhysics& operator=(GameEnginePhysics&& _Other) noexcept = delete;

protected:

private:
	static inline physx::PxPhysics* GetPhysics()
	{
		return m_pPhysics;
	}

	static inline physx::PxScene* GetCurrentScene()
	{
		return m_pScene;
	}

	static void SetIsPhysics(bool _Is)
	{
		IsPhysics = _Is;
	}

	static inline bool GetIsPhysics()
	{
		return IsPhysics;
	}

	static void SetGravity(float _gravity)
	{
		SceneGravity = _gravity;
	}

	static inline float GetSceneGravity()
	{
		return SceneGravity;
	}

	static void SetSceneGravity(float _gravity)
	{
		SceneGravity = _gravity;
		m_pScene->setGravity({ 0.0f, -SceneGravity, 0.0f });
	}

	static void CreatePhysicsX();
	static void Simulate(float _DeltaTime);
	static void ReleasePhysicsX();

	static bool RayCast(const float4& _vOrigin, const float4& _vDir, OUT float4& _vPoint, float _fDistance = 1000.f);
	static void ChangeScene(const std::string_view& _Name);
	static void CreateScene(const std::string_view& _Name);

	static bool IsPhysics;
	static float SceneGravity;

	static physx::PxDefaultAllocator m_Allocator;
	static UserErrorCallback m_ErrorCallback;

	static physx::PxPhysics* m_pPhysics;
	static physx::PxFoundation* m_pFoundation;

	static physx::PxPvd* m_pPvd;
	static physx::PxPvdTransport* m_pTransport;
	static physx::PxScene* m_pScene;
	static physx::PxDefaultCpuDispatcher* m_pDispatcher;
	static physx::PxMaterial* m_pMaterial;
	static physx::PxCooking* m_pCooking;

	static CustomCallback* CustomCallbackEvent;

	static std::map<std::string, physx::PxScene*> AllScene;

};

