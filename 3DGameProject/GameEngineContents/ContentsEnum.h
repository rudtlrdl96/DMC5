#pragma once

enum class DamageType
{
	None,
	VergilLight,	// 버질 약공 (경직이 매우 약함)
	Light,	// 약 공격
	Heavy,	// 강 공격	(날리는 공격, Blown)
	Air,	// 뛰우는 공격
	Snatch,	// 그랩 공격
	Slam,	// 공중에서 찍는 공격
	Buster,	// 네로 잡기 공격
	Stun,	// 경직 (저지먼트컷엔드)
};

enum class LogOrder
{
	Network,
	Enemy,
	Count
};

enum class Net_ActorType
{
	UNKNOWN,

	//플레이어
	Nero,
	Vergil,
	MirageBlade,

	//몬스터
	Empusa,
	HellCaina,
	GreenEmpusa,
	RedEmpusa,
	HellAntenora,

	//보스
	CavaliereAngelo,

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
	EventZone,
	DevilBreaker,
	RedOrb,
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
	ReflectionSetter,
	WallLight_On,
	WallLight_Off,
	StreetLight_On,
	StreetLight_Off,
	DecoLight,
	Fountain,
	StreetStall,
	Bench,
	RedSeal,
	StatueB,
	Location2_EnemySpawner0,
	Location2_EnemySpawner1,

};