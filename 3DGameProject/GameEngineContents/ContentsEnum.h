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
enum class DamageSoundType
{
	None,
	Sword,	// 검 (네로)
	Magic,	// 마법 (네로)
	Katana,	// 도 (버질)
	Blunt,	// 둔기(버질)
	Gun,	// 총, 원거리 공격
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
	HellAntenora,
	ScudoAngelo,
	ProtoAngelo,
	Qliphoth,

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
	Location2,
	Location11,
	Location19,
	NetTestLevel,
	TestStageLevel,

	UNKNOWN
};

enum class Net_EventType
{
	RedWallOff_Location2_Spawner0,
	RedWallOff_Location2_Spawner1,
	Location11_Start,
	Location11_End,
	Location19_Start,
	Location19_End,
	DamageCallBack,

	COUNT
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
	RedOrbLump,
	Shop,
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
	Item_RedOrb,
	Item_DevilBreaker_Overture,
	Item_DevilBreaker_Gerbera,
	Item_DevilBreaker_BusterArm,
	ElectronicBillboard,
	illminary,
	DivinityStatue,
	Item_RedOrbLump,
	ArcadeGate,
	ArcadeCafeRoof01,
	ArcadeCafeRoof02,
	StoreWhite01,
	StoreWood01,
	ToLocation11,
	ToLocation19,
	l02_03_StreetLine,
	FloatingIsland,
	FloatingIsland01,
	FloatingIsland02,
	tunnelWallLight,
};