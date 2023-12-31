#pragma once

// 설명 : PhysX 모듈에서 공통으로 사용할 함수들
class PhysXDefault
{
	typedef struct 
	{
		float x, y, z;
	} vector;

	typedef struct 
	{
		vector i, j, k;
	} matrix;

public:
	// constrcuter destructer
	PhysXDefault();
	~PhysXDefault();

	// delete Function
	PhysXDefault(const PhysXDefault& _Other) = delete;
	PhysXDefault(PhysXDefault&& _Other) noexcept = delete;
	PhysXDefault& operator=(const PhysXDefault& _Other) = delete;
	PhysXDefault& operator=(PhysXDefault&& _Other) noexcept = delete;

	// 필터링 셋팅
	void SetupFiltering(physx::PxShape* _Shape, physx::PxU32 _FilterGroup, physx::PxU32 _FilterMask);
	float4 ToEulerAngles(const physx::PxQuat& q);

	//정지마찰계수설정
	inline void SetStaticFriction(float _staticfriction)
	{
		Staticfriction = _staticfriction;
	}
	//운동마찰계수설정
	inline void SetDynamicFriction(float _dynamicfriction)
	{
		Dynamicfriction = _dynamicfriction;
	}
	//반발계수설정
	inline void SetRestitution(float _resitution)
	{
		Resitution = _resitution;
	}
	//순서대로 정지마찰계수설정, 운동마찰계수설정, 반발계수설정을 넣기
	inline void SetPhysxMaterial(float _staticfriction, float _dynamicfriction, float _resitution)
	{
		Staticfriction = _staticfriction;
		Dynamicfriction = _dynamicfriction;
		Resitution = _resitution;
	}

	//피벗설정
	inline void SetDynamicPivot(float4 _Pivot)
	{
		DynamicPivot = _Pivot;
	}

	inline void SetObstacleObject()
	{
		IsObstacle = true;
	}
	
	inline void SetGroundObject()
	{
		IsGround = true;
	}

	inline void SetWallObject()
	{
		IsWall = true;
	}

	inline void SetSlopeObject()
	{
		IsSlope = true;
	}

	inline void SetAggregateObj(bool _Flag)
	{
		IsAggregateObject = _Flag;
	}

	inline void AddActorAggregate(physx::PxActor* _Actor)
	{
		MapAggregate->addActor(*_Actor);
	}

	//쿼터니언 관련 함수
	float4 GetQuaternionEulerAngles(float4 rot);
	float4 GetQuaternionEulerAngles(physx::PxQuat rot);
	float4 NormalizeAngles(float4 angles);
	float NormalizeAngle(float angle);
	
	//로드리게스 회전 관련 함수
	float4 RodriguesRotate(vector p, vector v, float a);
	float dotProduct(vector v1, vector v2);
	vector matrixMultiply(matrix m, vector v);

protected:
	float Staticfriction = 0.0f;              // 정적마찰
	float Dynamicfriction = 0.0f;             // 동적마찰
	float Resitution = 0.0f;                  // 탄성계수

	float4 DynamicPivot = {0.0f, 0.0f, 0.0f};
	
	bool IsObstacle = false;
	bool IsGround = false;
	bool IsWall = false;
	bool IsSlope = false;
	bool IsAggregateObject = false;

	static physx::PxAggregate* MapAggregate;

private:
};

