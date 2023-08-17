#include "PrecompileHeader.h"
#include "EnemyActor_Boss.h"

EnemyActor_Boss::EnemyActor_Boss() 
{
}

EnemyActor_Boss::~EnemyActor_Boss() 
{
}

void EnemyActor_Boss::Start()
{
	EnemyTypeValue = EnemyType::Boss;
	BaseEnemyActor::Start();
}