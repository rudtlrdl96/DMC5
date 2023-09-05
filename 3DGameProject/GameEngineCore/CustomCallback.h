#pragma once

class PhysXCapsuleComponent;
class CustomCallback : public physx::PxSimulationEventCallback
{
public:
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {};
	void onWake(physx::PxActor** actors, physx::PxU32 count) override {};
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override {};
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
	void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override {};

	static void SetMainPlayer(PhysXCapsuleComponent* _MainPlayer)
	{
		MainPlayer = _MainPlayer;
	}

	std::function<void()> _Func;

private:
	static PhysXCapsuleComponent* MainPlayer;

};
