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
	Bench
};