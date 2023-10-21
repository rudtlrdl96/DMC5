#pragma once

enum class DamageType
{
	None,
	VergilLight,	// ���� ��� (������ �ſ� ����)
	Light,	// �� ����
	Heavy,	// �� ����	(������ ����, Blown)
	Air,	// �ٿ�� ����
	Snatch,	// �׷� ����
	Slam,	// ���߿��� ��� ����
	Buster,	// �׷� ��� ����
	Stun,	// ���� (������Ʈ�ƿ���)
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

	//�÷��̾�
	Nero,
	Vergil,
	MirageBlade,

	//����
	Empusa,
	HellCaina,
	GreenEmpusa,
	RedEmpusa,
	HellAntenora,

	//����
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
	Item_RedOrb,
	Item_DevilBreaker_Overture,
	Item_DevilBreaker_Gerbera,
	Item_DevilBreaker_BusterArm,
	ElectronicBillboard,
	illminary,
};