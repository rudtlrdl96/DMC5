#include "PrecompileHeader.h"
#include "TestStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "JudgementCut.h"
#include "ColorEffect.h"
#include "FXAA_Effect.h"
#include "ZoomEffect.h"

#include "Plane.h"
#include "TestObject.h"
#include "PlayerActor_Nero.h"

#include "ShaderTestActor.h"
#include "FreeCameraActor.h"

TestStageLevel* TestStageLevel::Inst = nullptr;

TestStageLevel::TestStageLevel()
{
	Inst = this;
}

TestStageLevel::~TestStageLevel()
{
	Inst = nullptr;
}

void TestStageLevel::Start()
{
	CreateActor<FreeCameraActor>();
	CreateActor<ShaderTestActor>()->GetTransform()->SetLocalPosition({ 0, 100, 0 });

	GetCamera(0)->ReflectionOn();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();

	StageBaseLevel::Start();

}

void TestStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void TestStageLevel::LevelChangeStart()
{
	//임시 불러오기, 추후 수정 예정
	StageBaseLevel::LevelChangeStart();
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"Location2.FBX"
		);

		GameEngineFBXMesh::Load(Path);
	}

	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"SkyBox.fbx"
		);

		GameEngineFBXMesh::Load(Path);
	}

	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"Location2ColGuide.fbx"
		);

		GameEngineFBXMesh::Load(Path);
	}

	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			},
			"TestCol.fbx"
		);

		GameEngineFBXMesh::Load(Path);
	}


	SetCamera({ 0,0,-500 });

	CreateStage(AllStageDatas[0]);

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->SetWorldPosition({ 0, 100, 0 });
}