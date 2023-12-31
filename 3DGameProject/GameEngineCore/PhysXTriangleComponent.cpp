#include "PrecompileHeader.h"
#include "PhysXTriangleComponent.h"

#include "GameEngineLevel.h"
#include "GameEngineFBXMesh.h"

PhysXTriangleComponent::PhysXTriangleComponent() 
{
}

PhysXTriangleComponent::~PhysXTriangleComponent() 
{
}

void PhysXTriangleComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXTriangleComponent::Update(float _DeltaTime)
{
	if (true == PositionSetFromParentFlag)
	{
		// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
		float4 tmpQuat = ParentActor.lock()->GetTransform()->GetWorldRotation().DegreeRotationToQuaternionReturn();

		physx::PxTransform tmpPxTransform
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z,
			physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
		);

		// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
		m_pStatic->setGlobalPose(tmpPxTransform);
	}
	else
	{
		// PhysX Actor의 상태에 맞춰서 부모의 Transform정보를 갱신
		float4 tmpWorldPos =
		{
			m_pStatic->getGlobalPose().p.x,
			m_pStatic->getGlobalPose().p.y,
			m_pStatic->getGlobalPose().p.z
		};

		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pStatic->getGlobalPose().q) * GameEngineMath::RadToDeg;

		ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
		ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
	}
}

void PhysXTriangleComponent::CreatePhysXActors(const std::string& _MeshName, bool _InverseIndex, float _Ratio /*= 1.f*/, float4 _GeoMetryRot /*= { 0.0f, 0.0f }*/)
{
	CustomFBXLoad(_MeshName, _Ratio, _InverseIndex);
	float4 tmpQuat = _GeoMetryRot.DegreeRotationToQuaternionReturn();

	m_pPhysics = GetLevel()->GetLevelPhysics();
	m_pScene = GetLevel()->GetLevelScene();
	m_pCooking = GetLevel()->GetCooking();

	// 부모 액터로부터 위치 생성
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat
		(
			tmpQuat.x,
			tmpQuat.y,
			tmpQuat.z,
			tmpQuat.w
		)
	);

	// 마찰, 탄성계수
	m_pMaterial = m_pPhysics->createMaterial(Staticfriction, Dynamicfriction, Resitution);

	// 충돌체의 종류
	m_pStatic = m_pPhysics->createRigidStatic(localTm);

	int RenderinfoCount = static_cast<int>(Mesh->GetRenderUnitCount());
	
	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//Vertex의 값을 Desc에 넣어준다
		//Size, 단위의 바이트사이즈, 데이터의 시작값
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = static_cast<physx::PxU32>(VertexVec[i].size());
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = &VertexVec[i][0];

		unsigned int IndexVecSize = static_cast<unsigned int>(IndexVec[i].size() / 3);

		//Index의 값을 Desc에 넣어준다
		//Triangle의 갯수(Index의 1/3개수), 단위의 바이트사이즈, 데이터의 시작값
		meshDesc.triangles.count = IndexVecSize;
		meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
		meshDesc.triangles.data = &IndexVec[i][0];

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum* result = nullptr;
		bool status = m_pCooking->cookTriangleMesh(meshDesc, writeBuffer, result);
		if (!status)
		{
			MsgAssert("매쉬를 불러와 피직스X 충돌체를 만드는데 실패했습니다 TriMesh");
		}

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* TriangleMesh = m_pPhysics->createTriangleMesh(readBuffer);
		//createExclusiveShapefh RigidStatic에 Shape를 넣어준다.
		m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pStatic, physx::PxTriangleMeshGeometry(TriangleMesh), *m_pMaterial);
		//피벗 설정
		physx::PxVec3 Pivot(DynamicPivot.x, DynamicPivot.y, DynamicPivot.z);
		m_pShape->setLocalPose(physx::PxTransform(Pivot));

		// invalid parameter : PxShape::setFlag(s): triangle mesh and heightfield triggers are not supported!
		// Triangle에서는 Trigger를 사용할 수 없음
		//shape_->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

		//충돌할때 필요한 필터 데이터
		if (true == IsObstacle)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle),
					static_cast<physx::PxU32>(PhysXFilterGroup::Player),
					0,
					0
				)
			);
		}
		else if (true == IsGround)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Ground),
					static_cast<physx::PxU32>(PhysXFilterGroup::Player),
					0,
					0
				)
			);
		}
		else if (true == IsWall)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Wall),
					static_cast<physx::PxU32>(PhysXFilterGroup::Player),
					0,
					0
				)
			);
		}
		else if (true == IsSlope)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Slope),
					static_cast<physx::PxU32>(PhysXFilterGroup::Player),
					0,
					0
				)
			);
		}
		else
		{
			std::string ParentName = GetActor()->GetName().data();
			MsgTextBox("충돌 플래그가 설정되지 않았습니다. " + ParentName);
		}
	}

	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start에서 피직스액터 생성하지 마세요\n2. 레벨에 CreateScene 하세요\n  오류가 뜬 레벨 이름 : " + LevelName);
	}

	PhysicsComponent = DynamicThis<PhysXTriangleComponent>();
	m_pShape->userData = GetActor();

	GetActor()->SetPhysicsActor();

	// Scene에 액터 추가
	if (true == IsAggregateObject)
	{
		AddActorAggregate(m_pStatic);
	}
	else
	{
		m_pScene->addActor(*m_pStatic);
	}
}

void PhysXTriangleComponent::CustomFBXLoad(const std::string& _MeshName, float _Ratio, bool _InverseIndex)
{
	//매쉬를 찾는다
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_MeshName);
	if (FindFBXMesh == nullptr)
	{
		MsgAssert("해당 매쉬는 존재하지 않습니다. " + _MeshName);
	}

	Mesh = FindFBXMesh;

	//랜더유닛카운트를 불러와 백터에 reserve를 한다
	size_t RenderinfoCount = Mesh->GetRenderUnitCount();

	VertexVec.reserve(RenderinfoCount + 1);
	IndexVec.reserve(RenderinfoCount + 1);

	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//i 번째 GetRenderUnit에서 RenderUnitInfo를 Get한다
		FbxRenderUnitInfo* RenderUnitInfo = Mesh->GetRenderUnit(i);

		std::vector<GameEngineVertex> MeshVertexs = RenderUnitInfo->Vertexs;
		std::vector<unsigned int> Indexes = RenderUnitInfo->Indexs[0];

		size_t VertexSize = MeshVertexs.size();
		size_t IndexSize = Indexes.size();
		std::vector<physx::PxVec3> InstVertVec;
		std::vector<unsigned int> InstIndexVec;
		//Vertex와 Index 정보를 VertexVec, IndexVec에 저장한다
		for (size_t j = 0; j < VertexSize; j++)
		{
			InstVertVec.push_back(physx::PxVec3(MeshVertexs[j].POSITION.x, MeshVertexs[j].POSITION.y, MeshVertexs[j].POSITION.z) * _Ratio);
		}

		if (_InverseIndex == true)
		{
			for (size_t j = 0; j < IndexSize; j++)
			{
				InstIndexVec.push_back(physx::PxU32(Indexes[j]));
			}
		}
		if (_InverseIndex == false)
		{
			for (size_t j = IndexSize - 1; j >= 0; --j)
			{
				InstIndexVec.push_back(physx::PxU32(Indexes[j]));
			}
		}

		VertexVec.push_back(InstVertVec);
		IndexVec.push_back(InstIndexVec);
	}
}

void PhysXTriangleComponent::SetWorldPosition(float4 _Value)
{
	physx::PxTransform CurTansform = m_pStatic->getGlobalPose();

	float ValueX = _Value.x;
	float ValueY = _Value.y;
	float ValueZ = _Value.z;

	CurTansform.p = { _Value.x, _Value.y, _Value.z };

	m_pStatic->setGlobalPose(CurTansform);
}

void PhysXTriangleComponent::AddWorldPosition(float4 _Value)
{
	physx::PxTransform CurTansform = m_pStatic->getGlobalPose();

	float ValueX = _Value.x;
	float ValueY = _Value.y;
	float ValueZ = _Value.z;

	CurTansform.p += { _Value.x, _Value.y, _Value.z };

	m_pStatic->setGlobalPose(CurTansform);
}

void PhysXTriangleComponent::SetWorldRotation(float4 _Value)
{
	float4 tmpQuat = _Value.DegreeRotationToQuaternionReturn();

	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pStatic->getGlobalPose().p, tmpPxQuat);

	m_pStatic->setGlobalPose(tmpTansform);
}

void PhysXTriangleComponent::AddWorldRotation(float4 _Value)
{
	float4 AddRptation = _Value;
	physx::PxQuat CurQuat = m_pStatic->getGlobalPose().q;
	float4 CurQuatToFloat4 = { CurQuat.x , CurQuat.y, CurQuat.z, CurQuat.w };
	float4 CurRotation = CurQuatToFloat4.QuaternionToEulerDeg();
	float4 ResultRotation = { CurRotation.x + AddRptation.x, CurRotation.y + AddRptation.y, CurRotation.z + AddRptation.z, CurRotation.w + AddRptation.w };

	float4 ResultsQuat = ResultRotation.DegreeRotationToQuaternionReturn();
	const physx::PxQuat tmpPxQuat(ResultsQuat.x, ResultsQuat.y, ResultsQuat.z, ResultsQuat.w);
	const physx::PxTransform tmpTansform(m_pStatic->getGlobalPose().p, tmpPxQuat);

	m_pStatic->setGlobalPose(tmpTansform);
}