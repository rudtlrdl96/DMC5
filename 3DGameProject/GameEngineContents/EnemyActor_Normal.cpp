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
	EnemyTypeValue = EnemyType::Normal;

	BaseEnemyActor::Start();

	//EnemyFSM.CreateState({ .StateValue = EnemyState::Basic,
	//	.Start = std::bind(&Idle_Enter, this),
	//	.Update = std::bind(&Idle_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Idle_Exit, this)});
	
	//EnemyFSM.CreateState({ .Name = "Chase",
	//	.Start = std::bind(&Chase_Enter, this),
	//	.Update = std::bind(&Chase_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Chase_Exit, this) });
	//
	//EnemyFSM.CreateState({ .Name = "Attack",
	//	.Start = std::bind(&Attack_Enter, this),
	//	.Update = std::bind(&Attack_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Attack_Exit, this) });
	//
	//EnemyFSM.CreateState({ .Name = "Fall",
	//	.Start = std::bind(&Fall_Enter, this),
	//	.Update = std::bind(&Fall_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Fall_Exit, this) });
	//
	//EnemyFSM.CreateState({ .Name = "Snatch",
	//	.Start = std::bind(&Snatch_Enter, this),
	//	.Update = std::bind(&Snatch_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Snatch_Exit, this) });
	//
	//EnemyFSM.CreateState({ .Name = "Buster",
	//	.Start = std::bind(&Buster_Enter, this),
	//	.Update = std::bind(&Buster_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Buster_Exit, this) });
	//
	//EnemyFSM.CreateState({ .Name = "Hit",
	//	.Start = std::bind(&Hit_Enter, this),
	//	.Update = std::bind(&Hit_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Hit_Exit, this) });
	//
	//EnemyFSM.CreateState({ .Name = "Death",
	//	.Start = std::bind(&Death_Enter, this),
	//	.Update = std::bind(&Death_Update, this, std::placeholders::_1),
	//	.End = std::bind(&Death_Exit, this) });
	//
	//EnemyFSM.ChangeState(EnemyState::Basic);
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

void EnemyActor_Normal::Death_Enter()
{
}

void EnemyActor_Normal::Death_Update(float _DeltaTime)
{
}

void EnemyActor_Normal::Death_Exit()
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