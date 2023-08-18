#pragma once
#include <GameEngineCore/GameEngineFSM.h>

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
	Normal,
	Boss,
};

enum class EnemySize
{
	Small,
	Middle,
	Large
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

	// 몬스터 피격 함수
	void MonsterHit(const EnemyHitData& _HitData);

	// 현재 몬스터가 슈퍼아머 상태인지 반환합니다. 만약 슈퍼아머 상태라면 그랩, 잡기등의 공격에 면역이됩니다.
	inline bool IsSuperArmor() const
	{
		return IsSpuerArmorValue;
	}

	// 몬스터의 종류를 반환합니다.
	inline EnemyCode GetEnemyCode() const
	{
		return EnemyCodeValue;
	}

	// 몬스터의 타입을 반환합니다. Normal, Boss 두 가지 타입이 있습니다.
	inline EnemyType GetEnemyType() const
	{
		return EnemyTypeValue;
	}

	// 몬스터의 크기를 반환합니다.
	inline EnemySize GetEnemySize() const
	{
		return EnemySizeValue;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	
	GameEngineFSM EnemyFSM;

	EnemyCode EnemyCodeValue = EnemyCode::Empusa;
	EnemyType EnemyTypeValue  = EnemyType::Normal;
	EnemySize EnemySizeValue = EnemySize::Small;

	bool IsSpuerArmorValue = false;
	bool IsSuperArmorValue = false;

	std::function<void()> SpuerArmorOn_Callback = nullptr;
	std::function<void()> SpuerArmorOff_Callback = nullptr;

	void SpuerArmorOn();
	void SuperArmorOff();

	virtual void MeshLoad() = 0;
	virtual void TextureLoad() = 0;
	virtual void AnimationLoad() = 0;
	virtual void CreateFSM() = 0;

private:
	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);
};