#include "PrecompileHeader.h"
#include "TestLevel.h"

//#include <GameEngineCore/ThirdParty/PhysX 5/inc/PxPhysicsAPI.h>

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

void TestLevel::Start()
{
	GetCamera(0)->SetProjectionType(CameraType::Orthogonal);

    //foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    //physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
    //gMaterial = physics->createMaterial(0.5f, 0.5f, 0.1f);  // 마찰, 탄성, 보통 부드러움 계수
}

void TestLevel::Update(float _DeltaTime)
{
	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is TestLevel");
	}

	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
	}

    //PhysXFunction();
}

void TestLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();
}

void TestLevel::LevelChangeEnd()
{
	IsMessage = false;
	GameEngineLevel::LevelChangeEnd();
}

//int TestLevel::PhysXFunction()
//{
//    // PhysX Foundation 초기화
//    physx::PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//    if (!foundation) {
//        std::cerr << "Error creating PhysX foundation!" << std::endl;
//        return 1;
//    }
//
//    // PhysX Physics 초기화
//    physx::PxPhysics* physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
//    if (!physics) {
//        std::cerr << "Error creating PhysX physics!" << std::endl;
//        return 1;
//    }
//
//    // 물리 시뮬레이션 시간 단위 설정
//    float timeStep = 1.0f / 60.0f;
//
//    // 물체 생성
//    physx::PxRigidDynamic* boxActor = physics->createRigidDynamic(physx::PxTransform(physx::PxIdentity));
//    physx::PxShape* boxShape = physics->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *gMaterial);
//    boxActor->attachShape(*boxShape);
//    boxShape->release();
//
//    // 물체를 시뮬레이션에 추가
//    physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
//    physx::PxScene* scene = physics->createScene(sceneDesc);
//    scene->addActor(*boxActor);
//
//    // 시뮬레이션 루프
//    for (int i = 0; i < 1000; ++i) {
//        scene->simulate(timeStep);
//        scene->fetchResults(true);
//        // 물체 위치, 회전 등을 업데이트하여 렌더링
//    }
//
//    // 자원 정리
//    scene->release();
//    physics->release();
//    foundation->release();
//
//    return 0;
//}