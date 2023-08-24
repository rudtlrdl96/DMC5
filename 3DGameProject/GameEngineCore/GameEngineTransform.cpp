#include "PrecompileHeader.h"
#include "GameEngineTransform.h"
#include "GameEngineObject.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"

std::function<bool(const CollisionData&, const CollisionData&)> GameEngineTransform::ArrColFunction[static_cast<int>(ColType::MAX)][static_cast<int>(ColType::MAX)];

class InitColFunction
{
public:
	InitColFunction()
	{
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::SPHERE3D)][static_cast<int>(ColType::SPHERE3D)] = &GameEngineTransform::SphereToSpehre;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::SPHERE3D)][static_cast<int>(ColType::AABBBOX3D)] = &GameEngineTransform::SphereToAABB;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::SPHERE3D)][static_cast<int>(ColType::OBBBOX3D)] = &GameEngineTransform::SphereToOBB;

		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::AABBBOX3D)][static_cast<int>(ColType::SPHERE3D)] = &GameEngineTransform::AABBToSpehre;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::AABBBOX3D)][static_cast<int>(ColType::AABBBOX3D)] = &GameEngineTransform::AABBToAABB;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::AABBBOX3D)][static_cast<int>(ColType::OBBBOX3D)] = &GameEngineTransform::AABBToOBB;

		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::OBBBOX3D)][static_cast<int>(ColType::SPHERE3D)] = &GameEngineTransform::OBBToSpehre;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::OBBBOX3D)][static_cast<int>(ColType::AABBBOX3D)] = &GameEngineTransform::OBBToAABB;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::OBBBOX3D)][static_cast<int>(ColType::OBBBOX3D)] = &GameEngineTransform::OBBToOBB;

		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::SPHERE2D)][static_cast<int>(ColType::SPHERE2D)] = &GameEngineTransform::Sphere2DToSpehre2D;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::SPHERE2D)][static_cast<int>(ColType::AABBBOX2D)] = &GameEngineTransform::Sphere2DToAABB2D;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::SPHERE2D)][static_cast<int>(ColType::OBBBOX2D)] = &GameEngineTransform::Sphere2DToOBB2D;

		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::AABBBOX2D)][static_cast<int>(ColType::SPHERE2D)] = &GameEngineTransform::AABB2DToSpehre2D;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::AABBBOX2D)][static_cast<int>(ColType::AABBBOX2D)] = &GameEngineTransform::AABB2DToAABB2D;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::AABBBOX2D)][static_cast<int>(ColType::OBBBOX2D)] = &GameEngineTransform::AABB2DToOBB2D;

		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::OBBBOX2D)][static_cast<int>(ColType::SPHERE2D)] = &GameEngineTransform::OBB2DToSpehre2D;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::OBBBOX2D)][static_cast<int>(ColType::AABBBOX2D)] = &GameEngineTransform::OBB2DToAABB2D;
		GameEngineTransform::ArrColFunction[static_cast<int>(ColType::OBBBOX2D)][static_cast<int>(ColType::OBBBOX2D)] = &GameEngineTransform::OBB2DToOBB2D;

	}
};

InitColFunction InitFunction;

void TransformData::LocalCalculation()
{
	Quaternion = Rotation.EulerDegToQuaternion();

	LocalWorldMatrix.Compose(Scale, Quaternion, Position);
}

void TransformData::WorldCalculation(const TransformData& _Parent, bool AbsoluteScale, bool AbsoluteRotation, bool AbsolutePosition)
{
	const float4x4& ParentMatrix = _Parent.WorldMatrix;
	WorldMatrix = WorldMatrix * ParentMatrix;

	if (true == AbsoluteScale || true == AbsoluteRotation || true == AbsolutePosition)
	{
		float4 WScale, WRotation, WPosition;
		float4 LScale, LRotation, LPosition;

		WorldMatrix.Decompose(WScale, WRotation, WPosition);

		LScale = Scale;

		if (true == AbsoluteScale)
		{
			WScale = Scale;
			LScale *= float4::GetSafeScaleReciprocal(_Parent.WorldScale, 0.00001f);
		}

		Quaternion = Rotation.EulerDegToQuaternion();

		if (true == AbsoluteRotation)
		{
			WRotation = Rotation.EulerDegToQuaternion();
			Quaternion = DirectX::XMQuaternionMultiply(Quaternion, DirectX::XMQuaternionInverse(_Parent.WorldQuaternion));
		}

		LPosition = Position;

		if (true == AbsolutePosition)
		{
			WPosition = Position;

			float4x4 InverseMat = _Parent.WorldMatrix.InverseReturn();
			LPosition *= InverseMat;
		}

		// 부모 재계산
		float4x4 MatScale, MatRot, MatPos;
		WorldMatrix.Compose(WScale, WRotation, WPosition);
		// 자식 재계산				
		ScaleMatrix.Scale(LScale);
		RotationMatrix = Quaternion.QuaternionToRotationMatrix();
		PositionMatrix.Pos(LPosition);

		LocalWorldMatrix.Compose(LScale, Quaternion, LPosition);
	}
}

void TransformData::SetViewAndProjection(const float4x4& _View, const float4x4& _Projection)
{
	View = _View;
	Projection = _Projection;
	WorldView = WorldMatrix * View;
	WorldViewProjectionMatrix = WorldMatrix * View * Projection;
}

bool GameEngineTransform::SphereToSpehre(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.SPHERE.Intersects(_Right.SPHERE);
}

bool GameEngineTransform::SphereToAABB(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.SPHERE.Intersects(_Right.AABB);
}

bool GameEngineTransform::SphereToOBB(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.SPHERE.Intersects(_Right.OBB);
}

bool GameEngineTransform::AABBToSpehre(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.AABB.Intersects(_Right.SPHERE);
}

bool GameEngineTransform::AABBToAABB(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.AABB.Intersects(_Right.AABB);
}

bool GameEngineTransform::AABBToOBB(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.AABB.Intersects(_Right.OBB);
}

bool GameEngineTransform::OBBToSpehre(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.OBB.Intersects(_Right.SPHERE);
}

bool GameEngineTransform::OBBToAABB(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.OBB.Intersects(_Right.AABB);
}

bool GameEngineTransform::OBBToOBB(const CollisionData& _Left, const CollisionData& _Right)
{
	return _Left.OBB.Intersects(_Right.OBB);
}

bool GameEngineTransform::Sphere2DToSpehre2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.SPHERE.Intersects(RightData.SPHERE);
}

bool GameEngineTransform::Sphere2DToAABB2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.SPHERE.Intersects(RightData.AABB);
}

bool GameEngineTransform::Sphere2DToOBB2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.SPHERE.Intersects(RightData.OBB);
}

bool GameEngineTransform::AABB2DToSpehre2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.AABB.Intersects(RightData.SPHERE);
}

bool GameEngineTransform::AABB2DToAABB2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.AABB.Intersects(RightData.AABB);
}

bool GameEngineTransform::AABB2DToOBB2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.AABB.Intersects(RightData.OBB);
}

bool GameEngineTransform::OBB2DToSpehre2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.OBB.Intersects(RightData.SPHERE);
}

bool GameEngineTransform::OBB2DToAABB2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.OBB.Intersects(RightData.AABB);
}

bool GameEngineTransform::OBB2DToOBB2D(const CollisionData& _Left, const CollisionData& _Right)
{
	CollisionData LeftData = _Left;
	LeftData.OBB.Center.z = 0.0f;
	CollisionData RightData = _Right;
	RightData.OBB.Center.z = 0.0f;

	return LeftData.OBB.Intersects(RightData.OBB);
}

GameEngineTransform::GameEngineTransform()
{
	TransformUpdate();
}

GameEngineTransform::~GameEngineTransform()
{
}

void GameEngineTransform::WorldCalculation()
{
	float4x4 ParentWorldMatrix = Parent->GetWorldMatrixRef();
	TransData.WorldCalculation(Parent->TransData, AbsoluteScale, AbsoluteRotation, AbsolutePosition);
}

void GameEngineTransform::LocalDecompose()
{
	TransData.LocalWorldMatrix.Decompose(TransData.LocalScale, TransData.LocalQuaternion, TransData.LocalPosition);
	TransData.LocalRotation = TransData.LocalQuaternion.QuaternionToEulerDeg();
}

float GameEngineTransform::GetRotation_Forward(const float4& _WolrdPosition)
{
	float4 Dir = (_WolrdPosition - TransData.WorldPosition).NormalizeReturn();
	float4 ForwardDir = GetWorldForwardVector();

	float4 Cross = float4::Cross3DReturnNormal(Dir, ForwardDir);

	if (Cross.y > 0)
	{
		Dir = -Dir;
	}

	float DotCos = acos(float4::DotProduct3D(Dir, ForwardDir));

	if (Cross.y > 0)
	{
		DotCos += GameEngineMath::PIE;
	}

	return DotCos * GameEngineMath::RadToDeg;
}

void GameEngineTransform::WorldDecompose()
{
	TransData.WorldMatrix.Decompose(TransData.WorldScale, TransData.WorldQuaternion, TransData.WorldPosition);
	TransData.WorldRotation = TransData.WorldQuaternion.QuaternionToEulerDeg();

	ColData.OBB.Center = TransData.WorldPosition.DirectFloat3;
	ColData.OBB.Extents = (TransData.WorldScale * 0.5f).DirectFloat3;
	ColData.OBB.Orientation = TransData.WorldQuaternion.DirectFloat4;
	ColData.ScaleABS();
}

void GameEngineTransform::SetParent(GameEngineTransform* _Parent, bool _IsParentWorld /*= true*/)
{
	if (IsDebug())
	{
		int a = 0;
	}

	if (nullptr == _Parent && nullptr == Master)
	{
		if (nullptr == dynamic_cast<GameEngineActor*>(Master))
		{
			MsgAssert("액터만이 루트 리스트에 추가될수 있습니다.");
			return;
		}
	}

	if (nullptr == Parent && nullptr == _Parent)
	{
		return;
	}

	// 내가 원래 기존의 부모를 가지고 있다면
	if (nullptr != Parent)
	{
		Parent->Child.remove(this);
		GameEngineObject* ParentMaster = Parent->Master;

		if (nullptr == ParentMaster)
		{
			MsgAssert("존재할수 없는 상황입니다 Master가 nullptr입니다");
			return;
		}

		std::shared_ptr<GameEngineObject> MasterPtr = Master->shared_from_this();
		ParentMaster->Childs.remove(MasterPtr);
		Parent = nullptr;

	}

	Parent = _Parent;

	if (nullptr != Parent)
	{
		if (true == _IsParentWorld)
		{
			TransData.LocalWorldMatrix = TransData.WorldMatrix * Parent->TransData.WorldMatrix.InverseReturn();
		}

		LocalDecompose();

		TransData.Position = TransData.LocalPosition;
		TransData.Rotation = TransData.LocalRotation;
		TransData.Scale = TransData.LocalScale;

		TransformUpdate();

		AbsoluteReset();

		GameEngineLevel* Level = Master->GetLevel();

		std::shared_ptr<GameEngineObject> MasterPtr = Master->shared_from_this();

		if (nullptr != dynamic_cast<GameEngineActor*>(Master))
		{
			Level->Actors[MasterPtr->GetOrder()].remove(std::dynamic_pointer_cast<GameEngineActor>(MasterPtr));
		}

		Parent->Child.push_back(this);
		Parent->Master->Childs.push_back(Master->shared_from_this());
	}
	else
	{
		WorldDecompose();

		TransData.Position = TransData.WorldPosition;
		TransData.Rotation = TransData.WorldRotation;
		TransData.Scale = TransData.WorldScale;
		TransformUpdate();
		AbsoluteReset();

		GameEngineLevel* Level = Master->GetLevel();

		std::shared_ptr<GameEngineObject> MasterPtr = Master->shared_from_this();

		if (nullptr != dynamic_cast<GameEngineActor*>(Master))
		{
			Level->Actors[MasterPtr->GetOrder()].push_back(std::dynamic_pointer_cast<GameEngineActor>(MasterPtr));
		}
		else
		{
			MsgAssert("액터만이 레벨의 루트 오브젝트로 지정될 수 있습니다.");
		}

	}
}

void GameEngineTransform::CalChild()
{
	for (GameEngineTransform* ChildTrans : Child)
	{
		ChildTrans->TransformUpdate();
		ChildTrans->CalChild();
	}
}

float4 GameEngineTransform::GetLocalPosition()
{
	return TransData.LocalPosition;
}

float4 GameEngineTransform::GetLocalScale()
{
	return TransData.LocalScale;
}

float4 GameEngineTransform::GetLocalRotation()
{
	return TransData.LocalRotation;
}

float4 GameEngineTransform::GetLocalQuaternion()
{
	return TransData.LocalQuaternion;
}

float4 GameEngineTransform::GetWorldPosition()
{
	return TransData.WorldPosition;
}

float4 GameEngineTransform::GetWorldScale()
{
	return TransData.WorldScale;
}

float4 GameEngineTransform::GetWorldRotation()
{
	return TransData.WorldRotation;
}

float4 GameEngineTransform::GetWorldQuaternion()
{
	return TransData.WorldQuaternion;
}

void GameEngineTransform::AbsoluteReset()
{
	AbsoluteScale = false;
	AbsoluteRotation = false;
	AbsolutePosition = false;
}

void GameEngineTransform::SetMaster(GameEngineObject* _Master)
{
	Master = _Master;
}

void GameEngineTransform::AllAccTime(float _DeltaTime)
{
	if (nullptr == Master)
	{
		return;
	}

	if (false == Master->IsUpdate())
	{
		return;
	}

	Master->AccLiveTime(_DeltaTime);

	for (GameEngineTransform* Trans : Child)
	{
		Trans->AllAccTime(_DeltaTime);
	}
}

void GameEngineTransform::AllUpdate(float _DeltaTime)
{
	if (nullptr == Master)
	{
		return;
	}

	if (false == Master->IsUpdate())
	{
		return;
	}

	Master->Update(_DeltaTime);

	for (GameEngineTransform* Trans : Child)
	{
		Trans->AllUpdate(_DeltaTime);
	}
}

void GameEngineTransform::AllRender(float _DeltaTime)
{
	if (nullptr == Master)
	{
		return;
	}

	if (false == Master->IsUpdate())
	{
		return;
	}

	Master->Render(_DeltaTime);

	for (GameEngineTransform* Trans : Child)
	{
		Trans->AllRender(_DeltaTime);
	}
}

void GameEngineTransform::AllRelease()
{
	if (nullptr == Master)
	{
		return;
	}

	if (false == Master->IsUpdate())
	{
		return;
	}

	Master->Release();

	for (GameEngineTransform* Trans : Child)
	{
		Trans->AllRelease();
	}
}

void GameEngineTransform::ChildRelease()
{
	std::list<GameEngineTransform*>::iterator ReleaseStartIter = Child.begin();
	std::list<GameEngineTransform*>::iterator ReleaseEndIter = Child.end();

	for (; ReleaseStartIter != ReleaseEndIter; )
	{
		GameEngineTransform* Trans = *ReleaseStartIter;

		if (nullptr == Trans->Master)
		{
			MsgAssert("몬가 잘못됨 도라에몽을 부르자.");
		}

		if (false == Trans->Master->IsDeath())
		{
			++ReleaseStartIter;
			continue;
		}

		ReleaseStartIter = Child.erase(ReleaseStartIter);
	}
}

bool GameEngineTransform::Collision(const CollisionParameter& Data)
{
	if (nullptr == Data._OtherTrans)
	{
		MsgAssert("nullptr인 트랜스 폼과는 충돌을 할수가 없습니다.");
	}

	return ArrColFunction[static_cast<int>(Data.ThisType)][static_cast<int>(Data.OtherType)](this->GetCollisionData(), Data._OtherTrans->GetCollisionData());
}

void GameEngineTransform::TransformUpdate()
{
	TransData.LocalCalculation();
	TransData.WorldMatrix = TransData.LocalWorldMatrix;

	if (nullptr != Parent)
	{
		WorldCalculation();
	}

	WorldDecompose();
	LocalDecompose();
}
