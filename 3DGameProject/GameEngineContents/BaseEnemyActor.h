#pragma once
#include <GameEngineCore/GameEngineFSM.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>

///	<param name="[AttackerPos] : ">공격 액터의 위치, 만약 ZERO일 경우 몬스터의 정면을 기준으로 애니메이션 실행 </param>
///	<param name="[Type] : ">공격의 타입 설정 </param>
///	<param name="[Damage] : ">플레이어 공격력 </param>
///	<param name="[ForceDir] : ">공격 물리량의 방향 자동으로 Normalize를 실행함 </param>
///	<param name="[ForcePower] : ">공격 물리량의 크기 </param>
class EnemyHitData
{
public:
	float4 AttackerPos = float4::ZERO;	
	MonsterDamageType Type = MonsterDamageType::None;
	float Damage = 0.0f;
	float4 ForceDir = float4::ZERO;
	float ForcePower = 1.0f;
};

enum class EnemyCode
{
	//초기화용
	None=100000,
	// 일반 몬스터
	Empusa = 0,
	GreenEmpusa = 1,
	RedEmpusa = 2,
	HellCaina = 1000,
	HellAntenora = 1001,

	// 보스 몬스터
	CavliereAngelo = 5000,
};

enum class EnemyType
{
	None,
	Normal,
	Fly,
	Boss,
};

enum class EnemySize
{
	None,
	Small,
	Middle,
	Large,
};

enum class EnemyHitDir
{
	None,
	Forward,
	Back,
	Left,
	Right,
};

enum class EnemyRotation
{
	Forward,
	Left,
	Left_90,
	Left_180,
	Right,
	Right_90,
	Right_180,
};

class BaseEnemyActor : public GameEngineActor, public GameEngineNetObject
{
	friend class EnemySpawnArea;
public:
	BaseEnemyActor();
	~BaseEnemyActor();

	BaseEnemyActor(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor(BaseEnemyActor&& _Other) noexcept = delete;
	BaseEnemyActor& operator=(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor& operator=(BaseEnemyActor&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent()
	{
		return PhysXCapsule;
	}

	// 몬스터 피격 함수(때렸을 때 호출해주시면 됩니다)
	void MonsterHit(const EnemyHitData& _HitData);
	void DamageColCheck();

	// 현재 몬스터가 슈퍼아머 상태인지 반환합니다. 만약 슈퍼아머 상태라면 그랩, 잡기등의 공격에 면역이됩니다.
	inline bool IsSuperArmor() const
	{
		return IsSuperArmorValue;
	}

	// 몬스터의 종류를 반환합니다.
	inline EnemyCode GetEnemyCode() const
	{
		return EnemyCodeValue;
	}

	// 몬스터의 타입을 반환합니다. Normal,Fly, Boss 세 가지 타입이 있습니다.
	inline EnemyType GetEnemyType() const
	{
		return EnemyTypeValue;
	}

	// 몬스터의 크기를 반환합니다.
	inline EnemySize GetEnemySize() const
	{
		return EnemySizeValue;
	}

	EnemyHitDir GetHitDir(const float4& _WolrdPos);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	// Enemy 모델링 메인 랜더러
	std::shared_ptr<class GameEngineFBXRenderer> EnemyRenderer = nullptr;

	// 물리 컴포넌트
	std::shared_ptr<PhysXCapsuleComponent> PhysXCapsule = nullptr;

	// Monster의 Collision
	//범위인식
	std::shared_ptr<class GameEngineCollision> RN_MonsterCollision = nullptr;
	//공격범위
	std::shared_ptr<class GameEngineCollision> MonsterAttackRange = nullptr;
	//자체Collision
	std::shared_ptr<class GameEngineCollision> MonsterCollision = nullptr;
	//Forward Collision
	std::shared_ptr<class GameEngineCollision> ForWardCollision = nullptr;
	std::shared_ptr<class GameEngineCollision> ColValue = nullptr;

	std::shared_ptr<class AttackCollision> MonsterAttackCollision = nullptr;

	GameEngineFSM EnemyFSM;

	//하위에서 설정해줘야하는 Data들=====================================================
	//Type+Data
	EnemyHitData HitData;
	EnemyCode EnemyCodeValue = EnemyCode::None;
	EnemyType EnemyTypeValue  = EnemyType::None;
	EnemySize EnemySizeValue = EnemySize::None;
	EnemyRotation EnemyRotationValue = EnemyRotation::Forward;
	//HP
	float EnemyHP = 0.0f;
	//Recognize(인식범위)
	float4 RN_Range = float4::ZERO;
	bool RN_Player = false;
	//공격범위
	float4 AttackRange = float4::ZERO;
	float MoveSpeed = 0.0f;

	// FSM 밸류
	int EnemyFSMValue = -1;
	//=================================================================================

	bool IsSuperArmorValue = false;
	//맞은방향
	EnemyHitDir HitDir = EnemyHitDir::None;

	std::function<void()> SuperArmorOn_Callback = nullptr;
	std::function<void()> SuperArmorOff_Callback = nullptr;

	float RotateValue = 0.0f;
	void ChasePlayer(float _DeltaTime);
	void SuperArmorOn();
	void SuperArmorOff();
	bool FloorCheck(float _Distance);

	float4 CrossMonsterAndPlayer();			 // 플레이어와 몬스터 외적
	float DotProductMonsterAndPlayer();      // 플레이어와 몬스터 내적
	void CheckHeadingRotationValue();		 // 내적, 외적 후 어떤 식으로 회전할지 결정
	float RotationToPlayerValue();			 // 몬스터가 플레이어에게 회전해야하는 정도

	virtual void EnemyMeshLoad() = 0;
	virtual void EnemyTypeLoad() = 0;
	virtual void EnemyAnimationLoad() = 0;
	virtual void EnemyCreateFSM() = 0;
	virtual void DamageCollisionCheck() = 0;

private:
	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);
};