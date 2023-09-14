#include "PrecompileHeader.h"
#include "MonsterTestLevel.h"

#include "TestObject.h"
#include "Plane.h"
#include "Enemy_Empusa.h"
#include "Enemy_GreenEmpusa.h"
#include "Enemy_RedEmpusa.h"
#include "Enemy_HellCaina.h"
#include "Enemy_HellAntenora.h"
#include "CavaliereAngelo.h"
#include "Ball.h"
#include "Wall.h"
#include "Slope.h"
#include <GameEngineCore/EngineGrid.h>

#include "Enemy_Empusa.h"
#include "PlayerActor_Nero.h"


MonsterTestLevel::MonsterTestLevel()
{
	
}

MonsterTestLevel::~MonsterTestLevel()
{

}

void MonsterTestLevel::Start()
{
	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50.0f, -100.0f });

	CreateScene(GetName());

}

void MonsterTestLevel::Update(float _DeltaTime)
{
	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is MonsterTest TestLevel");
	}

	if (true == GameEngineInput::IsDown("SelectLevel_02"))
	{
		EnemyHitData Datas;
		Datas.Type = MonsterDamageType::Air;
		TestMonster->MonsterHit(Datas);
	}
}

void MonsterTestLevel::LevelChangeStart()
{
	SetLevelSceneGravity(2000);
	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->GetPhysXComponent()->SetWorldPosition({ 0, 100, 1000 });
	Nero->SinglePlayLoad();

	std::shared_ptr<Enemy_Empusa> TestEmpusa = CreateActor<Enemy_Empusa>();
	

	std::shared_ptr<Plane> Flat = CreateActor<Plane>();

	std::shared_ptr<Wall> Flat2 = CreateActor<Wall>();
	Flat2->GetTransform()->AddWorldPosition(float4{ 300, 0, 300 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, 0, 90 });
	Flat2->GetTransform()->AddWorldRotation(float4{ 0, -40, 0 });
}

void MonsterTestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
