#include "PrecompileHeader.h"
#include "EnemyTestLevel.h"
#include "Enemy_HellCaina.h"

EnemyTestLevel::EnemyTestLevel() 
{
}

EnemyTestLevel::~EnemyTestLevel() 
{
}

void EnemyTestLevel::LevelChangeStart()
{
	std::shared_ptr<Enemy_HellCaina> NewEnemy = CreateActor<Enemy_HellCaina>();	
}