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

	//����


	//����Ʈ?

};

enum class Net_LevelType
{
	Intro,
	Title,
	SelectCharacter,

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
	Player,
	Enemy,

};