#pragma once

enum class DamageType
{
	None,
	Light,	// �� ����
	Medium,	// �� ����
	Heavy,	// �� ����	(������ ����, Blown)
	Air,	// �ٿ�� ����
	Snatch,	// �׷� ����
	Slam,	// ���߿��� ��� ����
	Buster,	// �׷� ��� ����
	Stun,	// ����
	VergilLight,	// ���� ��� (������ �ſ� ����)
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
	Test0,
	Test1,
};