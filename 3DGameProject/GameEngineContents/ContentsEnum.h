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