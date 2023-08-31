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
	
	EnemyFSM.CreateState
	(
		{ 
			.StateValue = EnemyState::M_Idle,
			.Start = std::bind(&EnemyActor_Normal::Idle_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Idle_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Idle_Exit, this)
		}
	);
	EnemyFSM.CreateState
	(
		{ 
			.StateValue = EnemyState::M_Chase,
			.Start = std::bind(&EnemyActor_Normal::Chase_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Chase_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Chase_Exit, this)
		}
	);
	EnemyFSM.CreateState
	(
		{ 
			.StateValue = EnemyState::M_Attack,
			.Start = std::bind(&EnemyActor_Normal::Attack_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Attack_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Attack_Exit, this)
		}
	);
	EnemyFSM.CreateState
	(
		{ 
			.StateValue = EnemyState::M_Fall,
			.Start = std::bind(&EnemyActor_Normal::Fall_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Fall_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Fall_Exit, this)
		}
	);
	EnemyFSM.CreateState
	(
		{
			.StateValue = EnemyState::M_Snatch,
			.Start = std::bind(&EnemyActor_Normal::Snatch_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Snatch_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Snatch_Exit, this)
		}
	);
	EnemyFSM.CreateState(
		{
			.StateValue = EnemyState::M_Buster,
			.Start = std::bind(&EnemyActor_Normal::Buster_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Buster_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Buster_Exit, this)
		}
	);
	EnemyFSM.CreateState
	(
		{
			.StateValue = EnemyState::M_Hit,
			.Start = std::bind(&EnemyActor_Normal::Hit_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Hit_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Hit_Exit, this)
		}
	);
	EnemyFSM.CreateState
	(
		{ 
			.StateValue = EnemyState::M_Death,
			.Start = std::bind(&EnemyActor_Normal::Death_Enter, this),
			.Update = std::bind(&EnemyActor_Normal::Death_Update, this, std::placeholders::_1),
			.End = std::bind(&EnemyActor_Normal::Death_Exit, this)
		}
	);
	EnemyFSM.ChangeState(EnemyState::M_Idle);
}

void EnemyActor_Normal::Update(float _DeltaTime)
{
	EnemyFSM.Update(_DeltaTime);
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