#pragma once

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
	//�÷��̾�
	Nero,
	Vergil,
	TestPlayer,

	//����


	//����Ʈ?

	COUNT
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
	Enemy,
	Wall,
	JumpWall,
	PlayerAttack,
	RN_Enemy, //Monster��Player�� �ν��ϴ� ����Collision
};

enum class DevilBreaker
{
	None,
	Overture,
	Gerbera,
	BusterArm
};