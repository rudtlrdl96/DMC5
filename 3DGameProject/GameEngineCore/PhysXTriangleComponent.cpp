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
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXTriangleComponent::Update(float _DeltaTime)
{
	if (true == PositionSetFromParentFlag)
	{
		// �θ��� Transform������ �������� PhysX Actor�� Ʈ�������� ����
		float4 tmpQuat = ParentActor.lock()->GetTransform()->GetWorldRotation().DegreeRotationToQuaternionReturn();

		physx::PxTransform tmpPxTransform
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z,
			physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
		);

		// �θ��� Transform������ �������� PhysX Actor�� Ʈ�������� ����
		m_pStatic->setGlobalPose(tmpPxTransform);
	}
	else
	{
		// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
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

	// �θ� ���ͷκ��� ��ġ ����
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

	// ����, ź�����
	m_pMaterial = m_pPhysics->createMaterial(Staticfriction, Dynamicfriction, Resitution);

	// �浹ü�� ����
	m_pStatic = m_pPhysics->createRigidStatic(localTm);

	int RenderinfoCount = static_cast<int>(Mesh->GetRenderUnitCount());
	
	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//Vertex�� ���� Desc�� �־��ش�
		//Size, ������ ����Ʈ������, �������� ���۰�
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = static_cast<physx::PxU32>(VertexVec[i].size());
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = &VertexVec[i][0];

		unsigned int IndexVecSize = static_cast<unsigned int>(IndexVec[i].size() / 3);

		//Index�� ���� Desc�� �־��ش�
		//Triangle�� ����(Index�� 1/3����), ������ ����Ʈ������, �������� ���۰�
		meshDesc.triangles.count = IndexVecSize;
		meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
		meshDesc.triangles.data = &IndexVec[i][0];

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum* result = nullptr;
		bool status = m_pCooking->cookTriangleMesh(meshDesc, writeBuffer, result);
		if (!status)
		{
			MsgAssert("�Ž��� �ҷ��� ������X �浹ü�� ����µ� �����߽��ϴ� TriMesh");
		}

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* TriangleMesh = m_pPhysics->createTriangleMesh(readBuffer);
		//createExclusiveShapefh RigidStatic�� Shape�� �־��ش�.
		m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pStatic, physx::PxTriangleMeshGeometry(TriangleMesh), *m_pMaterial);
		//�ǹ� ����
		physx::PxVec3 Pivot(DynamicPivot.x, DynamicPivot.y, DynamicPivot.z);
		m_pShape->setLocalPose(physx::PxTransform(Pivot));

		// invalid parameter : PxShape::setFlag(s): triangle mesh and heightfield triggers are not supported!
		// Triangle������ Trigger�� ����� �� ����
		//shape_->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

		//�浹�Ҷ� �ʿ��� ���� ������
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
			MsgTextBox("�浹 �÷��װ� �������� �ʾҽ��ϴ�. " + ParentName);
		}
	}

	if (m_pScene == nullptr)
	{
		std::string LevelName = GetLevel()->GetName().data();
		MsgAssert("1. Start���� ���������� �������� ������\n2. ������ CreateScene �ϼ���\n  ������ �� ���� �̸� : " + LevelName);
	}

	PhysicsComponent = DynamicThis<PhysXTriangleComponent>();
	m_pShape->userData = GetActor();

	GetActor()->SetPhysicsActor();

	// Scene�� ���� �߰�
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
	//�Ž��� ã�´�
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_MeshName);
	if (FindFBXMesh == nullptr)
	{
		MsgAssert("�ش� �Ž��� �������� �ʽ��ϴ�. " + _MeshName);
	}

	Mesh = FindFBXMesh;

	//��������ī��Ʈ�� �ҷ��� ���Ϳ� reserve�� �Ѵ�
	size_t RenderinfoCount = Mesh->GetRenderUnitCount();

	VertexVec.reserve(RenderinfoCount + 1);
	IndexVec.reserve(RenderinfoCount + 1);

	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//i ��° GetRenderUnit���� RenderUnitInfo�� Get�Ѵ�
		FbxRenderUnitInfo* RenderUnitInfo = Mesh->GetRenderUnit(i);

		std::vector<GameEngineVertex> MeshVertexs = RenderUnitInfo->Vertexs;
		std::vector<unsigned int> Indexes = RenderUnitInfo->Indexs[0];

		size_t VertexSize = MeshVertexs.size();
		size_t IndexSize = Indexes.size();
		std::vector<physx::PxVec3> InstVertVec;
		std::vector<unsigned int> InstIndexVec;
		//Vertex�� Index ������ VertexVec, IndexVec�� �����Ѵ�
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