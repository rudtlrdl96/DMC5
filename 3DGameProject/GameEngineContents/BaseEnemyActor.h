#pragma once
#include <GameEngineCore/GameEngineFSM.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "NetworkObjectBase.h"
#include "SoundController.h"
enum class EnemyCode
{
	//초기화용
	None = 100000,
	// 일반 몬스터
	Empusa = 0,
	HellCaina = 10,
	HellAntenora = 11,
	ScudoAngelo = 100,
	ProtoAngelo = 101,
	Qliphoth = 1000,

	// 보스 몬스터
	CavaliereAngelo = 5000,
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

class BaseEnemyActor : public NetworkObjectBase
{
	friend class EnemySpawnArea;
public:
	BaseEnemyActor();
	~BaseEnemyActor();

	BaseEnemyActor(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor(BaseEnemyActor&& _Other) noexcept = delete;
	BaseEnemyActor& operator=(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor& operator=(BaseEnemyActor&& _Other) noexcept = delete;

	void MinusEnemyHP(int _Value)
	{
		EnemyHP -= _Value;
		if (EnemyHP < 0)
		{
			DeathEvent();
		}
	}

	int GetHP()
	{
		return EnemyHP;
	}
	float GetHPRatio()
	{
		return (float)EnemyHP / (float)EnemyMaxHP;
	}

	// 몬스터 피직스 컴포넌트 리턴
	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent()
	{
		return PhysXCapsule;
	}

	// 패킷으로 전송받은 FSM을 클라이언트 FSM에 적용
	void SetFSMStateValue(int _StateValue)
	{
		EnemyFSM.ChangeState(_StateValue);
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

	//이 몬스터가 죽었을때 호출될 함수를 등록합니다.
	inline void PushDeathCallback(const std::function<void()>& _Callback)
	{
		DeathCallbacks.push_back(_Callback);
	}
	

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	virtual void EnemyTypeLoad() = 0;                                 // 기본 정보 입력 (HP, EnemyCode)
	virtual void EnemyMeshLoad() = 0;                                 // 매쉬 로드, 애니메이션 전에 실시한다.
	virtual void EnemyAnimationLoad() = 0;                            // 애니메이션 로드, 매쉬 로드 이후 실시한다.
	virtual void EnemyCreateFSM() = 0;                                // 서버, 싱글용 FSM 생성
	virtual void EnemyCreateFSM_Client() = 0;                         // 클라이언트용 FSM 생성
	virtual void DamageCollisionCheck(float _DeltaTime) = 0;          // 서버, 싱글에서 처리할 데미지 콜리전 체크 함수
	virtual void DamageCollisionCheck_Client(float _DeltaTime) = 0;   // 클라이언트에서 처리할 데미지 콜리전 체크 함수
	virtual void RecognizeCollisionCheck(float _DeltaTime) = 0;
	virtual void DeathCheck();
	virtual void PlayDamageEvent(DamageType _Type , DamageSoundType _SoundType);

	void DeathEvent();
	void RedOrbDrop();													// 사망시 실행. 레드오브를 뿌립니다

	std::shared_ptr<class GameEngineFBXRenderer> EnemyRenderer = nullptr;         // 랜더러
																			      
	std::shared_ptr<class GameEngineFBXRenderer> LeftWeapon = nullptr;            // 무기가 있으면 이용
	std::shared_ptr<class GameEngineFBXRenderer> RightWeapon = nullptr;           // 무기가 있으면 이용
																			      
	std::shared_ptr<class PhysXCapsuleComponent> PhysXCapsule = nullptr;          // 피직스 컴포넌트
	std::shared_ptr<class GameEngineCollision> MonsterCollision = nullptr;        // 몬스터 자체 콜리전
	std::shared_ptr<class AttackCollision> MonsterAttackCollision = nullptr;      // 몬스터 공격 콜리전
	std::shared_ptr<class AttackCollision> MonsterAttackCollision_Two = nullptr;  // 몬스터 공격 콜리전
	std::shared_ptr<class GameEngineCollision> RN_MonsterCollision = nullptr;     // 몬스터 공격 범위 인식 콜리전
	std::shared_ptr<class Item_EnemyRedOrb> RedOrbs = nullptr;					  // 죽을때 드랍되는 레드오브
	std::shared_ptr<class FXSystem> MonsterEffectRenderer = nullptr;			  // 이펙트

	//하위에서 설정해줘야하는 Data들=====================================================
	EnemyCode EnemyCodeValue = EnemyCode::None;
	//HP
	int EnemyMaxHP = 0;
	int EnemyHP = 0;
	// FSM 밸류
	int EnemyFSMValue = -1;

	bool DeathValue = false;
	bool DeathSettig = false;
	bool IsBurn = false;
	float DeathColor = 0.0f;

	void RendererBurn(float _DeltaTime);
	//=================================================================================

	//====================================================
	// 플레이어 세팅
	void PlayerCheckInit();                                                          // 몬스터 최초 생성 시 플레이어 정보를 class BasePlayerActor* Player에 저장
	bool PlayerContactCheck(float _DeltaTime, GameEngineCollision* _Collision);      // RN_MonsterCollision에 충돌한 충돌체(플레이어) 정보를 class BasePlayerActor* Player에 저장
	bool PlayerContactCheck_Normal(GameEngineCollision* _Collision);
	void PlayerAttackCheck(GameEngineCollision* _Collision);                         // 자신을 공격한 충돌체(플레이어) 정보를 class BasePlayerActor* Player에 저장

	GameEngineFSM EnemyFSM;
	SoundController Sound;
	class BasePlayerActor* Player = nullptr;
	float AppearDelayTime = 0.0f;
	float ContactDelayTime = 0.0f;
	int ServerPlayerID = 0;
	bool IsClientHit = false;
	bool IsTimeStop = false;
	bool TimeStop = false;
	bool IsChangeState = false;
	//====================================================

	//====================================================
	// 몬스터 바닥 체크 (RayCast)
	bool FloorCheck(float _Distance);    // 액터 위치에서 아래 방향으로 Raycast를 실시하여 바닥(피직스 충돌체)을 체크한다.
	float FallDistance = 0.0f;           // FloorCheck() 시 필요, 각자의 몬스터 Start 부분에서 값 적용
	//====================================================

	//====================================================
	// 랜더 쉐이크
	float4 CurRenderPosition = float4::ZERO;
	bool IsRenderShaking = false;
	float RenderShakeTime = 0.0f;
	short ShakingCameraSetting = 0;
	short ShakingCount = 0;
	short ShakingMaxCount = 0;

	void RenderShake(float _DeltaTime);  // 슈퍼아머 상태에서 히트 시 랜더러를 좌우로 흔든다.
	//====================================================

	//====================================================
	// Slerp 계산에 사용
	EnemyRotation EnemyRotationValue = EnemyRotation::Forward;  // 몬스터가 어떤 방향으로 회전해야 하는지에 대한 값
	EnemyHitDirect EnemyHitDirValue = EnemyHitDirect::Forward;  //

	float4 CurRotation = float4::ZERO;                          // Slerp 시작 전 현재 각도
	float4 GoalRotation = float4::ZERO;                         // Slerp 시작 시 목표로 하는 각도
	float4 RotationValue = float4::ZERO;                        // Slerp실행 시 계산된 결과값(현재 각도)
	float4 SnatchStartPosition = float4::ZERO;                  // Snatch 시작 전 현재 위치
	float4 SnatchEndPosition = float4::ZERO;				    // Snatch 시작 시 목표로 하는 위치
	float SnatchTime = 0.0f;								    // Snatch 시작 시 소요된 시간 (Lerp 시간)
	float SlerpTime = 0.0f;									    // Slerp 시작 시 소요된 시간 (Slerp 시간)
	float DotProductValue = 0.0f;                               // MonsterAndPlayerDotProductDegree() 실행 후 내적 결과값
	bool IsSnatch = false;									    // true로 변경 시 Snatch 실시, 끝나면 false로 변경됨
	bool AnimationTurnStart = false;						    // true로 변경 시 랜더러가 턴 됐을 때 RotationCheck()를 실시할지 결정, 히트 시 false로 변경되며 몬스터를 해당 위치로 바로 회전시킴

	float4 MonsterAndPlayerCross();			         // Player에 대해 현재 몬스터의 위치를 외적합니다. (어디로 돌 것인지 결정하기 위함)
	float MonsterAndPlayerDotProduct();              // Player에 대해 현재 몬스터의 위치를 내적합니다. (해당 방향으로 몇도를 회전해야 하는지 알기 위함)
	float MonsterAndPlayerDotProductDegree();        // Player에 대해 현재 몬스터의 위치를 내적합니다. Degree로 값을 반환해줍니다.
	void RotationCheck();            		         // Player에 대해 현재 몬스터의 위치를 외적, 내적을 한 뒤 어떤 방향으로 회전해야하는지를 EnemyRotation EnemyRotationValue에 저장합니다.
	void AttackDirectCheck();                        // Player에 대해 현재 몬스터의 위치를 외적, 내적을 한 뒤 어떤 방향에서 공격 받았는지를 EnemyHitDirect EnemyHitDirValue에 저장합니다.
	void SlerpCalculation();				         // Slerp로 회전해야 하는 경우에 대한 값을 계산합니다. 계산 결과는 float4 CurRotation, float4 GoalRotation에 저장됩니다. Y축에 대한 회전만 따집니다.
	void SlerpTurn(float _DeltaTime);                // 일정 시간 동안 SlerpCalculation()()로 계산된  값을 향해 몬스터를 Slerp시킨다. 값이 실시간으로 float4 RotationValue에 저장된다.
	void BusterCalculation(float4 _attachposition);  // 버스터 히트 시 몬스터를 Player FBX Bone의 어떤 위치에 부착할지 결정한다.
	void BusterEnd();                                // 버스터 종료 시 몬스터 랜더러의 위치를 원래 위치로 되돌린다.
	void SnatchCalculation();                        // 스내치 히트 시 Lerp할 몬스터와 Player의 위치값을 float4 SnatchStartPosition, float4 SnatchEndPosition에 값을 저장한다.
	void MonsterSnatch(float _DeltaTime);            // 일정 시간(1.0f) 동안 SnatchCalculation()로 계산된 값을 향해 몬스터를 Lerp시킨다.
	//====================================================

	//====================================================
	// TimeScale Action
public:
	void HitStop(DamageType _Type);  // 입력된 DamageType에 따라 일정시간동안 타임스케일을 낮춥니다
	void StopTime(float _DeltaTime); // 일정시간동안 타임스케일을 0으로 합니다. _Time 초 후 타임스케일은 1이 됩니다. CapsuleCollision은 Off됩니다
	//====================================================
protected:
	//====================================================
	// 방향 설정
	float4 ForwardDirect = float4::ZERO;  // 몬스터의 전방
	float4 BackDirect = float4::ZERO;     // 몬스터의 후방
	float4 RightDirect = float4::ZERO;	  // 몬스터의 우측
	float4 LeftDirect = float4::ZERO;	  // 몬스터의 좌측
	float4 PushDirect = float4::ZERO;	  // 몬스터의 날아갈 방향(후방)

	void AllDirectSetting();              // 몬스터의 현재 위치와 각도에 대해 전방, 후방, 좌측, 우측에 대한 노말 벡터값을 결정해준다. Phsysics와 Engine 연산이 1프레임 차이나기 때문에 벡터를 임의로 회전시킨다.
	void AllDirectSetting_Normal();       // 몬스터의 현재 위치와 각도에 대해 전방, 후방, 좌측, 우측에 대한 노말 벡터값을 결정해준다.
	void PushDirectSetting();             // 몬스터의 현재 위치와 각도에 대해 날아갈 방향을 PushDirect에 노말벡터로 저장한다.
	
	/// <summary>
	/// 몬스터를 전방으로 addforce 실시
	/// </summary>
	void SetAdvance(float _Value)
	{
		PhysXCapsule->SetPush(ForwardDirect * _Value);
	}

	/// <summary>
	/// 몬스터를 후방으로 addforce 실시 (BackDirect 사용), 걷거나 회전에 대해 뒤로 밀려날 때 사용
	/// </summary>
	void SetThrowback(float _Value)
	{
		SetMoveStop();
		PhysXCapsule->SetPush(BackDirect * _Value);
	}

	/// <summary>
	/// 몬스터를 후방으로 addforce 실시 (PushDirect 사용), 히트 시 날리기는 해당 로직 사용
	/// </summary>
	void SetPush(float _Value)
	{
		SetMoveStop();
		PhysXCapsule->SetPush(PushDirect * _Value);
	}

	/// <summary>
	/// 공중에서 Y+ 방향으로 addforce 실시, 작용 전에 현재 물리(속도)값을 초기화시킨 후 적용
	/// </summary>
	void SetAir(float _Value)
	{
		PhysXCapsule->SetAirState(_Value);
	}

	/// <summary>
	/// 몬스터 전방으로 이동
	/// </summary>
	void SetForwardMove(float _Value)
	{
		PhysXCapsule->SetMove(ForwardDirect * _Value);
	}

	/// <summary>
	/// 몬스터 후방으로 이동
	/// </summary>
	void SetBackMove(float _Value)
	{
		PhysXCapsule->SetMove(BackDirect * _Value);
	}

	/// <summary>
	/// 몬스터 우측으로 이동
	/// </summary>
	void SetRightMove(float _Value)
	{
		PhysXCapsule->SetMove(RightDirect * _Value);
	}

	/// <summary>
	/// 몬스터 좌측으로 이동
	/// </summary>
	void SetLeftMove(float _Value)
	{
		PhysXCapsule->SetMove(LeftDirect * _Value);
	}

	/// <summary>
	/// 모든 힘, 속도값을 0으로 만듦
	/// </summary>
	void SetMoveStop()
	{
		PhysXCapsule->SetLinearVelocityZero();
	}

private:
	std::vector<std::function<void()>> DeathCallbacks;
};