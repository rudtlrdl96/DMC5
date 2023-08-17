#include "PrecompileHeader.h"
#include "EnemyActor_Normal.h"

EnemyActor_Normal::EnemyActor_Normal()
{
}

EnemyActor_Normal::~EnemyActor_Normal()
{
}

void EnemyActor_Normal::Start()
{
	BaseEnemyActor::Start();

	//std::function<void()> Start = std::bind(&Idle_Enter, this);
	//std::function<void(float)> Update = std::bind(&Idle_Update, this, std::placeholder::_1);
}

void EnemyActor_Normal::Idle_Enter()
{
}

void EnemyActor_Normal::Idle_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Idle_Exit()
{
}

void EnemyActor_Normal::Chase_Enter()
{
}

void EnemyActor_Normal::Chase_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Chase_Exit()
{
}

void EnemyActor_Normal::Attack_Enter()
{
}

void EnemyActor_Normal::Attack_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Attack_Exit()
{
}

void EnemyActor_Normal::Hit_Enter()
{
}

void EnemyActor_Normal::Hit_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Hit_Exit()
{
}

void EnemyActor_Normal::Fall_Enter()
{
}

void EnemyActor_Normal::Fall_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Fall_Exit()
{
}

void EnemyActor_Normal::Snatch_Enter()
{
}

void EnemyActor_Normal::Snatch_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Snatch_Exit()
{
}

void EnemyActor_Normal::Buster_Enter()
{
}

void EnemyActor_Normal::Buster_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Buster_Exit()
{
}