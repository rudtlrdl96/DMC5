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
	//플레이어
	Nero,
	Vergil,

	//몬스터


	//이펙트?

	COUNT
};

enum class Net_LevelType
{
	Intro,
	Title,
	SelectCharacter,

	StartStage,
	BossStage,
	//TODO
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
};