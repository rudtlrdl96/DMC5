#include "PrecompileHeader.h"
#include "GameEnginePhysics.h"

physx::PxDefaultAllocator GameEnginePhysics::gAllocator;
physx::PxDefaultErrorCallback GameEnginePhysics::gErrorCallback;

physx::PxFoundation* GameEnginePhysics::foundation = nullptr;
physx::PxPhysics* GameEnginePhysics::physics = nullptr;
physx::PxMaterial* GameEnginePhysics::material = nullptr;
physx::PxScene* GameEnginePhysics::scene = nullptr;

GameEnginePhysics::GameEnginePhysics() 
{
}

GameEnginePhysics::~GameEnginePhysics() 
{
}

void GameEnginePhysics::Initialize()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());

	material = physics->createMaterial(0.5f, 0.5f, 0.1f);

	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	scene = physics->createScene(sceneDesc);
}

void GameEnginePhysics::Render(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime)
{
	physx::PxRigidDynamic* box = PxCreateDynamic(*physics, physx::PxTransform(physx::PxVec3(0, 10, 0)), physx::PxBoxGeometry(1, 1, 1), *material, 10.0f);

	scene->addActor(*box);
	scene->simulate(_DeltaTime);
	scene->fetchResults(true);
}

void GameEnginePhysics::Release()
{
	scene->release();
	physics->release();
	foundation->release();
}