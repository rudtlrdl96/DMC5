#pragma once
#include <list>
#include <memory>
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineObjectBase.h"

enum class ColType
{
	// ĸ��
	// 2D������ �浹�� ��ΰ� ������ ���ƾ� �Ѵ�.
	SPHERE2D, // z�� 0���� ����� �浹
	AABBBOX2D, // z�� 0���� ����� �浹
	OBBBOX2D, // z�� 0���� ����� �浹
	SPHERE3D,
	AABBBOX3D,
	OBBBOX3D,
	MAX,
};

class CollisionData
{
public:
	union
	{
		DirectX::BoundingSphere SPHERE;
		DirectX::BoundingBox AABB;
		DirectX::BoundingOrientedBox OBB;
	};

	void ScaleABS()
	{
		OBB.Extents.x = abs(OBB.Extents.x);
		OBB.Extents.y = abs(OBB.Extents.y);
		OBB.Extents.z = abs(OBB.Extents.z);
	}

	CollisionData()
	{

	}
};

class CollisionParameter
{
public:
	class GameEngineTransform* _OtherTrans = nullptr;
	ColType ThisType = ColType::SPHERE3D;
	ColType OtherType = ColType::SPHERE3D;
};

struct TransformData
{
	// ����� ���� ���ǿ� ������
	float4 Scale;
	float4 Rotation;
	float4 Quaternion;
	float4 Position;

	float4 LocalScale;
	float4 LocalRotation;
	float4 LocalQuaternion;
	float4 LocalPosition;

	float4 WorldScale;
	float4 WorldRotation;
	float4 WorldQuaternion;
	float4 WorldPosition;

	float4x4 ScaleMatrix;
	float4x4 RotationMatrix;
	float4x4 PositionMatrix;
	float4x4 LocalWorldMatrix;
	float4x4 WorldMatrix;
	float4x4 View;
	float4x4 Projection;
	float4x4 ViewPort;
	float4x4 WorldViewProjectionMatrix;

	void WorldCalculation(const TransformData& _Parent, bool AbsoluteScale, bool AbsoluteRotation, bool AbsolutePosition);
	void LocalCalculation();

	void SetViewAndProjection(const float4x4& _View, const float4x4& _Projection);

public:
	TransformData()
	{
		Scale = float4::One;
		Rotation = float4::Null;
		Quaternion = float4::Null;
		Position = float4::Zero;
		//WorldScale = float4::One;
		//WorldRotation = float4::Null;
		//WorldQuaternion = float4::Null;
		//WorldPosition = float4::Zero;
	}
};

// ���� : Ư���� ��ü�� ũ�� ȸ�� �̵��� ���õ� ���ϼӼ��� �������ش�.
class GameEngineObject;
class GameEngineTransform : public GameEngineObjectBase
{
	friend class GameEngineObject;
	friend class GameEngineLevel;

public:
	// constrcuter destructer
	GameEngineTransform();
	~GameEngineTransform();

	// delete Function
	GameEngineTransform(const GameEngineTransform& _Other) = delete;
	GameEngineTransform(GameEngineTransform&& _Other) noexcept = delete;
	GameEngineTransform& operator=(const GameEngineTransform& _Other) = delete;
	GameEngineTransform& operator=(GameEngineTransform&& _Other) noexcept = delete;

	void SetLocalPositiveScaleX()
	{
		TransData.Scale.x = abs(TransData.Scale.x);
		SetLocalScale(TransData.Scale);
	}

	void SetLocalNegativeScaleX()
	{
		TransData.Scale.x = -abs(TransData.Scale.x);
		SetLocalScale(TransData.Scale);
	}

	void SetWorldScale(const float4& _Value)
	{
		AbsoluteScale = true;
		TransData.Scale = _Value;

		TransformUpdate();
		CalChild();
	}

	void SetWorldRotation(const float4& _Value)
	{
		AbsoluteRotation = true;
		TransData.Rotation = _Value;

		TransformUpdate();
		CalChild();
	}

	void SetWorldPosition(const float4& _Value)
	{
		AbsolutePosition = true;
		TransData.Position = _Value;

		TransformUpdate();
		CalChild();
	}

	void SetLocalScale(const float4& _Value)
	{
		AbsoluteScale = false;
		TransData.Scale = _Value;

		TransformUpdate();
		CalChild();
	}

	void SetLocalRotation(const float4& _Value)
	{
		AbsoluteRotation = false;
		TransData.Rotation = _Value;

		TransformUpdate();
		CalChild();
	}

	void SetLocalPosition(const float4& _Value)
	{
		AbsolutePosition = false;
		TransData.Position = _Value;


		TransformUpdate();
		CalChild();
	}

	void AddLocalScale(const float4& _Value)
	{
		SetLocalScale(TransData.Scale + _Value);
	}

	void AddLocalRotation(const float4& _Value)
	{
		SetLocalRotation(TransData.Rotation + _Value);
	}

	void AddLocalPosition(const float4& _Value)
	{
		SetLocalPosition(TransData.Position + _Value);
	}

	void AddWorldScale(const float4& _Value)
	{
		SetWorldScale(TransData.Scale + _Value);
	}

	void AddWorldRotation(const float4& _Value)
	{
		SetWorldRotation(TransData.Rotation + _Value);
	}

	void AddWorldPosition(const float4& _Value)
	{
		SetWorldPosition(TransData.Position + _Value);
	}


	float4 GetWorldForwardVector()
	{
		return TransData.WorldMatrix.ArrVector[2].NormalizeReturn();
	}

	float4 GetWorldUpVector()
	{
		return TransData.WorldMatrix.ArrVector[1].NormalizeReturn();
	}

	float4 GetWorldRightVector()
	{
		return TransData.WorldMatrix.ArrVector[0].NormalizeReturn();
	}

	float4 GetWorldBackVector()
	{
		return -GetWorldForwardVector();
	}

	float4 GetWorldDownVector()
	{
		return -GetWorldUpVector();
	}

	float4 GetWorldLeftVector()
	{
		return -GetWorldRightVector();
	}

	float4 GetLocalPosition();
	float4 GetLocalScale();
	float4 GetLocalRotation();
	float4 GetLocalQuaternion();
	float4 GetWorldPosition();
	float4 GetWorldScale();
	float4 GetWorldRotation();
	float4 GetWorldQuaternion();


	float4 GetLocalForwardVector()
	{
		return TransData.LocalWorldMatrix.ArrVector[2].NormalizeReturn();
	}

	float4 GetLocalUpVector()
	{
		return TransData.LocalWorldMatrix.ArrVector[1].NormalizeReturn();
	}

	float4 GetLocalRightVector()
	{
		return TransData.LocalWorldMatrix.ArrVector[0].NormalizeReturn();
	}

	float4x4 GetLocalWorldMatrix()
	{
		return TransData.LocalWorldMatrix;
	}

	const float4x4& GetLocalWorldMatrixRef()
	{
		return TransData.LocalWorldMatrix;
	}

	const float4x4 GetWorldMatrix()
	{
		return TransData.WorldMatrix;
	}

	const float4x4& GetWorldMatrixRef()
	{
		return TransData.WorldMatrix;
	}

	const float4x4 GetWorldViewProjectionMatrix()
	{
		return TransData.WorldViewProjectionMatrix;
	}

	const float4x4& GetWorldViewProjectionMatrixRef()
	{
		return TransData.WorldViewProjectionMatrix;
	}

	inline const void SetCameraMatrix(const float4x4& _View, const float4x4& _Projection)
	{
		TransData.SetViewAndProjection(_View, _Projection);
	}

	inline const void SetViewPort(const float4x4& _ViewPort)
	{
		TransData.ViewPort = _ViewPort;
		TransData.WorldViewProjectionMatrix *= TransData.ViewPort;
	}

	void CalChild();

	void SetParent(GameEngineTransform* _Parent, bool _IsParentWorld = true);

	bool IsAbsoluteScale()
	{
		return  AbsoluteScale;
	}
	bool IsAbsoluteRotation()
	{
		return  AbsoluteRotation;
	}
	bool IsAbsolutePosition()
	{
		return  AbsolutePosition;
	}



	GameEngineTransform* GetParent()
	{
		return Parent;
	}

	const TransformData& GetTransDataRef()
	{
		return TransData;
	}

	void SetTransformData(const TransformData& _Data)
	{
		TransData = _Data;
	}

	CollisionData GetCollisionData()
	{
		return ColData;
	}

protected:

private:

	void WorldDecompose();
	void LocalDecompose();

	void WorldCalculation();

	void AbsoluteReset();

	void TransformUpdate();

	TransformData TransData;

	bool AbsoluteScale = false;
	bool AbsoluteRotation = false;
	bool AbsolutePosition = false;

	GameEngineTransform* Parent = nullptr;
	std::list<GameEngineTransform*> Child;

private:
	void AllAccTime(float _DeltaTime);

	void AllUpdate(float _DeltaTime);

	void AllRender(float _DeltaTime);

	void AllRelease();

	void ChildRelease();

	void SetMaster(GameEngineObject* _Master);

	GameEngineObject* GetMaster()
	{
		return Master;
	}

	GameEngineObject* Master = nullptr;


public:
	bool Collision(const CollisionParameter& Data);

private:
	friend class InitColFunction;

	CollisionData ColData;

	static std::function<bool(const CollisionData&, const CollisionData&)> ArrColFunction[static_cast<int>(ColType::MAX)][static_cast<int>(ColType::MAX)];

public:
	static bool SphereToSpehre(const CollisionData&, const CollisionData&);
	static bool SphereToAABB(const CollisionData&, const CollisionData&);
	static bool SphereToOBB(const CollisionData&, const CollisionData&);

	static bool AABBToSpehre(const CollisionData&, const CollisionData&);
	static bool AABBToAABB(const CollisionData&, const CollisionData&);
	static bool AABBToOBB(const CollisionData&, const CollisionData&);

	static bool OBBToSpehre(const CollisionData&, const CollisionData&);
	static bool OBBToAABB(const CollisionData&, const CollisionData&);
	static bool OBBToOBB(const CollisionData&, const CollisionData&);

	static bool Sphere2DToSpehre2D(const CollisionData&, const CollisionData&);
	static bool Sphere2DToAABB2D(const CollisionData&, const CollisionData&);
	static bool Sphere2DToOBB2D(const CollisionData&, const CollisionData&);

	static bool AABB2DToSpehre2D(const CollisionData&, const CollisionData&);
	static bool AABB2DToAABB2D(const CollisionData&, const CollisionData&);
	static bool AABB2DToOBB2D(const CollisionData&, const CollisionData&);

	static bool OBB2DToSpehre2D(const CollisionData&, const CollisionData&);
	static bool OBB2DToAABB2D(const CollisionData&, const CollisionData&);
	static bool OBB2DToOBB2D(const CollisionData&, const CollisionData&);

};

