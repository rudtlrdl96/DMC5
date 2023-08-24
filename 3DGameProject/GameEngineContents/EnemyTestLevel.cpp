#include "PrecompileHeader.h"
#include "EnemyTestLevel.h"
#include "Enemy_HellCaina.h"
#include "Enemy_Empusa.h"
#include "Plane.h"

EnemyTestLevel::EnemyTestLevel() 
{
}

EnemyTestLevel::~EnemyTestLevel() 
{
}

void EnemyTestLevel::Start()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
}

void EnemyTestLevel::LevelChangeStart()
{
	std::shared_ptr<Enemy_Empusa> NewEnemy = CreateActor<Enemy_Empusa>();
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
}