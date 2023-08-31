#include "PrecompileHeader.h"
#include "PhysXConvexComponent.h"

#include "GameEngineLevel.h"

PhysXConvexComponent::PhysXConvexComponent() 
{
}

PhysXConvexComponent::~PhysXConvexComponent() 
{
}

void PhysXConvexComponent::CreatePhysXActors(const std::string& _MeshName, physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxCooking* _cooking, bool _InverseIndex, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot, bool _Gravity)
{
	CustomFBXLoad(_MeshName);
	float4 tmpQuat = _GeoMetryRot.DegreeRotationToQuaternionReturn();

	m_pPhysics = _physics;
	m_pScene = _Scene;

	// 시소 위치 고정
	SeesawPos = physx::PxVec3
	(
		ParentActor.lock()->GetTransform()->GetWorldPosition().x
		, ParentActor.lock()->GetTransform()->GetWorldPosition().y
		, ParentActor.lock()->GetTransform()->GetWorldPosition().z
	);

	// 부모 액터로부터 위치 생성
	physx::PxTransform localTm
	(
		SeesawPos,
		physx::PxQuat
		(
			tmpQuat.x, 
			tmpQuat.y, 
			tmpQuat.z, 
			tmpQuat.w
		)
	);

	// 마찰, 탄성계수
	//material_ = _physics->createMaterial(0.0f, 0.0f, 0.0f);	

	m_pMaterial = _physics->createMaterial(Staticfriction, Dynamicfriction, Resitution);

	// TODO::배율을 적용할 경우 이쪽 코드를 사용
	//float4 tmpMagnification = { SIZE_MAGNIFICATION_RATIO };
	//physx::PxVec3 tmpGeoMetryScale(_GeoMetryScale.x * tmpMagnification.x * 0.5f, 
	//							   _GeoMetryScale.y * tmpMagnification.y * 0.5f, 
	//							   _GeoMetryScale.z * tmpMagnification.z * 0.5f);

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	const physx::PxVec3 convexVerts[] = 
	{ 
		physx::PxVec3(0,1,0),
		physx::PxVec3(1,0,0),
		physx::PxVec3(-1,0,0),
		physx::PxVec3(0,0,1),
		physx::PxVec3(0,0,-1) 
	};

	physx::PxConvexMeshDesc convexDesc;
	convexDesc.points.count = static_cast<physx::PxU32>(VertexVec.size());
	convexDesc.points.stride = sizeof(physx::PxVec3);
	convexDesc.points.data = &VertexVec[0];
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream buf;
	physx::PxConvexMeshCookingResult::Enum result;
	if (!_cooking->cookConvexMesh(convexDesc, buf, &result))
	{
		MsgAssert("매쉬를 불러와 피직스X 충동체를 만드는데 실패했습니다");
	}
	physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	physx::PxConvexMesh* convexMesh = _physics->createConvexMesh(input);

	// 충돌체의 종류
	m_pDynamic = _physics->createRigidDynamic(localTm);
	//중력 flag // true시 중력받지않음
	m_pDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, _Gravity);
	// 충돌체의 형태
	// 충돌체의 크기는 절반의 크기를 설정하므로 실제 Renderer의 스케일은 충돌체의 2배로 설정되어야 함
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pDynamic, physx::PxConvexMeshGeometry(convexMesh), *m_pMaterial);

	//피벗 설정
	physx::PxVec3 Pivot(DynamicPivot.x, DynamicPivot.y, DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	// 밀도 설정
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pDynamic, 0.01f);

	// 제동비율
	m_pDynamic->setAngularDamping(physx::PxReal(0.05f));

	//shape_->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle)
	//		, 0, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0));

	//충돌할때 필요한 필터 데이터
	if (IsObstacle == true)
	{
		m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle)
			, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0));
	}

	else if (IsGround == true)
	{
		m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Ground)
			, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0));
	}


	m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);

	// 제동비율
	//dynamic_->setAngularDamping(physx::PxReal(0.05f));

	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start에서 피직스액터 생성하지 마세요\n2. 레벨에 CreateScene 하세요\n  오류가 뜬 레벨 이름 : " + LevelName);
	}

	// Scene에 액터 추가
	if (true == IsAggregateObject)
	{
		AddActorAggregate(m_pDynamic);
	}
	else
	{
		m_pScene->addActor(*m_pDynamic);
	}

	//// 시소를 받칠 발판 생성
	//physx::PxRigidStatic* PlaneStatic = _physics->createRigidStatic(physx::PxTransform(physx::PxVec3(0.0f, -15.0f, 0.0f)));
	//physx::PxRigidActorExt::createExclusiveShape(*PlaneStatic, physx::PxBoxGeometry(physx::PxVec3(30.0f, 0.5f, 3.0f)), *material_);

	//// Scene에 액터 추가
	//_Scene->addActor(*PlaneStatic);
}

void PhysXConvexComponent::AddForce(float4 _Force)
{
	m_pDynamic->addForce(physx::PxVec3(_Force.x, _Force.y, _Force.z), physx::PxForceMode::eIMPULSE);
}

void PhysXConvexComponent::AddTorque(float4 _Torque)
{
	m_pDynamic->addTorque(physx::PxVec3(_Torque.x, _Torque.y, _Torque.z), physx::PxForceMode::eFORCE);
}

void PhysXConvexComponent::AddAngularVelocity(float4 _Velocity)
{
	m_pDynamic->setAngularVelocity(physx::PxVec3(_Velocity.x, _Velocity.y, _Velocity.z));
}

void PhysXConvexComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXConvexComponent::Update(float _DeltaTime)
{
	m_pDynamic->addForce(AddUpdateForce, physx::PxForceMode::eFORCE);

	if (PositionSetFromParentFlag == true)
	{
		float4 tmpQuat = ParentActor.lock()->GetTransform()->GetWorldRotation().DegreeRotationToQuaternionReturn();

		physx::PxTransform tmpPxTransform
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z, 
			physx::PxQuat
			(
				tmpQuat.x, 
				tmpQuat.y, 
				tmpQuat.z, 
				tmpQuat.w
			)
		);

		// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
		m_pDynamic->setGlobalPose(tmpPxTransform);
	}
	else
	{
		// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
		float4 tmpWorldPos = 
		{ 
			m_pDynamic->getGlobalPose().p.x, 
			m_pDynamic->getGlobalPose().p.y, 
			m_pDynamic->getGlobalPose().p.z
		};

		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

		ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
		ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
	}
}

void PhysXConvexComponent::CustomFBXLoad(const std::string& _MeshName)
{
	//GameEngineDirectory Dir;

	//Dir.MoveParentToExitsChildDirectory(DIR_RESOURCES);
	//Dir.Move(DIR_RESOURCES);
	//Dir.Move(DIR_PHYSXMESH);
	//std::string Path = Dir.PlusFilePath(_MeshName);

	//std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_MeshName);
	//if (FindFBXMesh == nullptr)
	//{
	//	Mesh = GameEngineFBXMesh::Load(Path);
	//}
	//else
	//{
	//	Mesh = FindFBXMesh;
	//}

	//FbxRenderUnitInfo* RenderUnitInfo = Mesh->GetRenderUnit(0);

	//std::vector<GameEngineVertex> MeshVertexs = RenderUnitInfo->Vertexs;

	//size_t VertexSize = MeshVertexs.size();

	//VertexVec.reserve(VertexSize + 1);

	//for (size_t i = 0; i < VertexSize; i++)
	//{
	//	VertexVec.push_back(physx::PxVec3(MeshVertexs[i].POSITION.x, MeshVertexs[i].POSITION.y, MeshVertexs[i].POSITION.z));
	//}
}
