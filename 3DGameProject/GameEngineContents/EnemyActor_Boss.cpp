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
	EnemyCodeValue = EnemyCode::CavliereAngelo;
	EnemyTypeValue = EnemyType::Boss;
	EnemySizeValue = EnemySize::Large;
	BaseEnemyActor::Start();
}