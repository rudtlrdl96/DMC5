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
	TestMonster = CreateActor<Enemy_Empusa>();
}

void MonsterTestLevel::LevelChangeEnd()
{
	IsMessage = false;
}
