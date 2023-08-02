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
    //gMaterial = physics->createMaterial(0.5f, 0.5f, 0.1f);  // ����, ź��, ���� �ε巯�� ���
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
//    // PhysX Foundation �ʱ�ȭ
//    physx::PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//    if (!foundation) {
//        std::cerr << "Error creating PhysX foundation!" << std::endl;
//        return 1;
//    }
//
//    // PhysX Physics �ʱ�ȭ
//    physx::PxPhysics* physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
//    if (!physics) {
//        std::cerr << "Error creating PhysX physics!" << std::endl;
//        return 1;
//    }
//
//    // ���� �ùķ��̼� �ð� ���� ����
//    float timeStep = 1.0f / 60.0f;
//
//    // ��ü ����
//    physx::PxRigidDynamic* boxActor = physics->createRigidDynamic(physx::PxTransform(physx::PxIdentity));
//    physx::PxShape* boxShape = physics->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *gMaterial);
//    boxActor->attachShape(*boxShape);
//    boxShape->release();
//
//    // ��ü�� �ùķ��̼ǿ� �߰�
//    physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
//    physx::PxScene* scene = physics->createScene(sceneDesc);
//    scene->addActor(*boxActor);
//
//    // �ùķ��̼� ����
//    for (int i = 0; i < 1000; ++i) {
//        scene->simulate(timeStep);
//        scene->fetchResults(true);
//        // ��ü ��ġ, ȸ�� ���� ������Ʈ�Ͽ� ������
//    }
//
//    // �ڿ� ����
//    scene->release();
//    physics->release();
//    foundation->release();
//
//    return 0;
//}