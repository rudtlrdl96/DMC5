#pragma once
#include <GameEngineCore/GameEngineFSM.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>


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

	void SetFSMStateValue(int _StateValue)
	{
		EnemyFSM_Client.ChangeState(_StateValue);
	}

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

	std::shared_ptr<class GameEngineCollision> ColValue = nullptr;

	std::shared_ptr<class AttackCollision> MonsterAttackCollision = nullptr;

	GameEngineFSM EnemyFSM;
	GameEngineFSM EnemyFSM_Client;

	//하위에서 설정해줘야하는 Data들=====================================================
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

	float RotationValue = 0.0f;  // CheckHeadingRotationValue() 실행 후 내적 결과값
	float FallDistance = 0.0f;   // FloorCheck() 시 필요, 각자의 몬스터 Start 부분에서 값 적용

	void SuperArmorOn();
	void SuperArmorOff();
	bool FloorCheck(float _Distance);

	float4 CrossMonsterAndPlayer();			 // 플레이어와 몬스터 외적
	float DotProductMonsterAndPlayer();      // 플레이어와 몬스터 내적 (그냥 내적)
	float RotationToPlayerValue();			 // 플레이어와 몬스터 내적값 deg로 반환 (오른쪽 +, 왼쪽 -, 180도 까지)
	void CheckHeadingRotationValue();		 // 내적, 외적 후 어떤 식으로 회전할지 결정
	void SlerpCalculation();				 // slerp 조건 계산
	void SlerpTurn(float _DeltaTime);        // slerp로 턴

	//====================================================
	// Slerp 계산에 사용
	float4 CurRotation = float4::ZERO;
	float4 GoalRotation = float4::ZERO;
	float4 RotValue = float4::ZERO;     // 계산된 결과값
	float SlerpTime = 0.0f;
	//====================================================
	virtual void EnemyMeshLoad() = 0;
	virtual void EnemyTypeLoad() = 0;
	virtual void EnemyAnimationLoad() = 0;
	virtual void EnemyCreateFSM() = 0;
	virtual void EnemyCreateFSM_Client() = 0;
	virtual void DamageCollisionCheck(float _DeltaTime) = 0;

	void Update_ProcessPacket() override;
	void Update_SendPacket(float _DeltaTime) override;

	//====================================================
	// 서버 패킷 관련
	float4 Server_PrePosition = float4::ZERO;
	float4 Server_NextPosition = float4::ZERO;
	float4 Server_Rotation = float4::ZERO;
	float Sever_Timer = 0.0f;
	//====================================================

private:

};