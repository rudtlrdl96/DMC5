#pragma once

// ���� : PhysX ��⿡�� �������� ����� �Լ���
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

	// ���͸� ����
	void SetupFiltering(physx::PxShape* _Shape, physx::PxU32 _FilterGroup, physx::PxU32 _FilterMask);
	float4 ToEulerAngles(const physx::PxQuat& q);

	//���������������
	inline void SetStaticFriction(float _staticfriction)
	{
		Staticfriction = _staticfriction;
	}
	//������������
	inline void SetDynamicFriction(float _dynamicfriction)
	{
		Dynamicfriction = _dynamicfriction;
	}
	//�ݹ߰������
	inline void SetRestitution(float _resitution)
	{
		Resitution = _resitution;
	}
	//������� ���������������, ������������, �ݹ߰�������� �ֱ�
	inline void SetPhysxMaterial(float _staticfriction, float _dynamicfriction, float _resitution)
	{
		Staticfriction = _staticfriction;
		Dynamicfriction = _dynamicfriction;
		Resitution = _resitution;
	}

	//�ǹ�����
	inline void SetDynamicPivot(float4 _Pivot)
	{
		DynamicPivot = _Pivot;
	}

	//������Ʈ�� ���͵����͸� �ֱ����� bool���� �����ϴ� �Լ�
	inline void SetObjectObstacle()
	{
		IsObstacle = true;
	}
	
	//������Ʈ�� ���͵����͸� �ֱ����� bool���� �����ϴ� �Լ�
	inline void SetObjectGround()
	{
		IsGround = true;
	}

	inline void SetAggregateObj(bool _Flag)
	{
		IsAggregateObject = _Flag;
	}

	inline void AddActorAggregate(physx::PxActor* _Actor)
	{
		MapAggregate->addActor(*_Actor);
	}

	//���ʹϾ� ���� �Լ�
	float4 GetQuaternionEulerAngles(float4 rot);
	float4 GetQuaternionEulerAngles(physx::PxQuat rot);
	float4 NormalizeAngles(float4 angles);
	float NormalizeAngle(float angle);
	
	//�ε帮�Խ� ȸ�� ���� �Լ�
	float4 RodriguesRotate(vector p, vector v, float a);
	double dotProduct(vector v1, vector v2);
	vector matrixMultiply(matrix m, vector v);

protected:
	float Staticfriction = 0.0f;              // ��������
	float Dynamicfriction = 0.0f;             // ��������
	float Resitution = 0.0f;                  // ź�����

	float4 DynamicPivot = {0.0f, 0.0f, 0.0f};
	
	bool IsObstacle = false;
	bool IsGround = false;
	bool IsAggregateObject = false;

	static physx::PxAggregate* MapAggregate;

private:
};

