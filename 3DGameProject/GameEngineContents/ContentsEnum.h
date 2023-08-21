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

enum class ContentsActorType
{
	//플레이어
	Nero,
	Vergil,

	//몬스터


	//이펙트?

};

enum class ContentsLevelType
{
	Intro,
	Title,
	SelectCharacter,

	//TODO
};