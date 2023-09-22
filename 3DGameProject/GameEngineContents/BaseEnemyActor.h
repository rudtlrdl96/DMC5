#pragma once
#include <GameEngineCore/GameEngineFSM.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "NetworkObjectBase.h"

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

enum class EnemyHitDirect
{
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

class BaseEnemyActor : public GameEngineActor, public NetworkObjectBase
{
	friend class EnemySpawnArea;
public:
	BaseEnemyActor();
	~BaseEnemyActor();

	BaseEnemyActor(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor(BaseEnemyActor&& _Other) noexcept = delete;
	BaseEnemyActor& operator=(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor& operator=(BaseEnemyActor&& _Other) noexcept = delete;

	// 몬스터 피직스 컴포넌트 리턴
	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent()
	{
		return PhysXCapsule;
	}

	// 패킷으로 전송받은 FSM을 클라이언트 FSM에 적용
	void SetFSMStateValue(int _StateValue)
	{
		EnemyFSM_Client.ChangeState(_StateValue);
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

	void StartRenderShaking(short _ShakingMaxCount)
	{
		ShakingMaxCount = _ShakingMaxCount;
		IsRenderShaking = true;
	}

	void StartMonsterSnatch()
	{
		SnatchCalculation();
		IsSnatch = true;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	virtual void EnemyMeshLoad() = 0;
	virtual void EnemyTypeLoad() = 0;
	virtual void EnemyAnimationLoad() = 0;
	virtual void EnemyCreateFSM() = 0;
	virtual void EnemyCreateFSM_Client() = 0;
	virtual void DamageCollisionCheck(float _DeltaTime) = 0;
	virtual void RecognizeCollisionCheck(float _DeltaTime) = 0;

	std::shared_ptr<class GameEngineFBXRenderer> EnemyRenderer = nullptr;     // 랜더러
	std::shared_ptr<PhysXCapsuleComponent> PhysXCapsule = nullptr;            // 피직스 컴포넌트
	std::shared_ptr<class GameEngineCollision> MonsterCollision = nullptr;    // 몬스터 자체 콜리전
	std::shared_ptr<class AttackCollision> MonsterAttackCollision = nullptr;  // 몬스터 공격 콜리전
	std::shared_ptr<class GameEngineCollision> RN_MonsterCollision = nullptr; // 몬스터 공격 범위 인식 콜리전

	GameEngineFSM EnemyFSM;        // 싱글, 서버용 FSM
	GameEngineFSM EnemyFSM_Client; // 클라이언트용 FSM (패킷 전송 받아서 변경됨)

	//하위에서 설정해줘야하는 Data들=====================================================
	EnemyCode EnemyCodeValue = EnemyCode::None;
	EnemyType EnemyTypeValue  = EnemyType::None;
	EnemySize EnemySizeValue = EnemySize::None;

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

	void SuperArmorOn();
	void SuperArmorOff();
	bool IsSuperArmorValue = false;

	std::function<void()> SuperArmorOn_Callback = nullptr;
	std::function<void()> SuperArmorOff_Callback = nullptr;
	
	//====================================================
	// 몬스터 바닥 체크 (RayCast)
	bool FloorCheck(float _Distance);
	float FallDistance = 0.0f;     // FloorCheck() 시 필요, 각자의 몬스터 Start 부분에서 값 적용
	//====================================================

	//====================================================
	// 랜더 쉐이크
	float4 CurRenderPosition = float4::ZERO;
	bool IsRenderShaking = false;
	float RenderShakeTime = 0.0f;
	short ShakingCameraSetting = 0;
	short ShakingCount = 0;
	short ShakingMaxCount = 0;

	void RenderShake(float _DeltaTime);
	//====================================================

	//====================================================
	// Slerp 계산에 사용
	EnemyRotation EnemyRotationValue = EnemyRotation::Forward;
	EnemyHitDirect EnemyHitDirValue = EnemyHitDirect::Forward;

	float4 CurRotation = float4::ZERO;
	float4 GoalRotation = float4::ZERO;
	float4 RotationValue = float4::ZERO;     // Slerp 계산된 결과값
	float SlerpTime = 0.0f;
	float DotProductValue = 0.0f;            // CheckHeadingRotationValue() 실행 후 내적 결과값
	float AnimationRotation = 0.0f;
	float AnimationRotationTime = 0.0f;
	float CalFrame = 0.0f;
	UINT StartFrame = 0;
	UINT EndFrame = 0;
	bool AnimationTurnStart = false;

	float4 MonsterAndPlayerCross();			 // 플레이어와 몬스터 외적
	float MonsterAndPlayerDotProduct();      // 플레이어와 몬스터 내적 (그냥 내적)
	float MonsterAndPlayerDotProductDegree();// 플레이어와 몬스터 내적값 deg로 반환 (오른쪽 +, 왼쪽 -, 180도 까지)
	void RotationCheck();            		 // 내적, 외적 후 어떤 식으로 회전할지 결정
	void AttackDirectCheck();                // 플레이어 공격 방향 체크
	void SlerpCalculation();				 // slerp 조건 계산
	void SlerpTurn(float _DeltaTime);        // slerp로 턴
	void AnimationSlerpCalculation();
	void AnimationSlerpTurn(float _DeltaTime);
	//====================================================

	//====================================================
	// 방향 설정
	float4 ForwardDirect = float4::ZERO;
	float4 BackDirect = float4::ZERO;
	float4 RightDirect = float4::ZERO;
	float4 LeftDirect = float4::ZERO;
	float4 PushDirect = float4::ZERO;
	float4 SnatchStartPosition = float4::ZERO;
	float4 SnatchEndPosition = float4::ZERO;
	float SnatchTime = 0.0f;
	bool IsSnatch = false;

	void AllDirectSetting();
	void PushDirectSetting();
	void SnatchCalculation();
	void MonsterSnatch(float _DeltaTime);
	
	void SetAdvance(float _Value)
	{
		PhysXCapsule->SetPush(ForwardDirect * _Value);
	}

	void SetThrowback(float _Value)
	{
		PhysXCapsule->SetPush(BackDirect * _Value);
	}

	void SetPush(float _Value)
	{
		PhysXCapsule->SetPush(PushDirect * _Value);
	}

	void SetAir(float _Value)
	{
		PhysXCapsule->SetAirState(_Value);
	}

	void SetForwardMove(float _Value)
	{
		PhysXCapsule->SetMove(ForwardDirect * _Value);
	}

	void SetBackMove(float _Value)
	{
		PhysXCapsule->SetMove(BackDirect * _Value);
	}

	void SetRightMove(float _Value)
	{
		PhysXCapsule->SetMove(RightDirect * _Value);
	}

	void SetLeftMove(float _Value)
	{
		PhysXCapsule->SetMove(LeftDirect * _Value);
	}

	void SetMoveStop()
	{
		PhysXCapsule->SetLinearVelocityZero();
	}
	//====================================================

	//====================================================
	

	//====================================================

private:

};