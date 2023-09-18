#pragma once

enum class DamageType
{
	None,
	Light,	// 약 공격
	Medium,	// 중 공격
	Heavy,	// 강 공격	(날리는 공격, Blown)
	Air,	// 뛰우는 공격
	Snatch,	// 그랩 공격
	Slam,	// 공중에서 찍는 공격
	Buster,	// 네로 잡기 공격
	Stun,	// 경직
};

enum class MonsterDamageType
{
	None,
	AngleDamage,
	Air,
	Snatch,
	Blown,
	Drilling,
	Launching,
	Slam,
	Downward,
	Stun,
	Hold
};

enum class LogOrder
{
	Network,
	Enemy,
	Count
};

enum class Net_ActorType
{
	//플레이어
	Nero,
	Vergil,
	TestPlayer,

	//몬스터
	Empusa,
	HellCaina,



	//이펙트?

	COUNT,
};

enum class PlayerType
{
	None,
	Nero,
	Vergil,
};

enum class Net_LevelType
{
	StartStage,
	BossStage,
	NetTestLevel,

	UNKNOWN
};

enum class ActorOrder
{
	Player,
	Enemy,
	Map
};

enum class CollisionOrder
{
	Null = -1,
	Player,
	PlayerLockOn,
	Enemy,
	PlayerAttack,
	EnemyAttack,
	RN_Enemy,
	RN_Player,
	CullingCol,
	GameEngineRenderer,
};

enum class DevilBreaker
{
	None,
	Overture,
	Gerbera,
	BusterArm
};

enum class FieldMapObjType
{
	Null = -1,
	Test0,
	Test1,
};