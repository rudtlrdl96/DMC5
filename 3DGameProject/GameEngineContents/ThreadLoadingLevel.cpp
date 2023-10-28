#include "PrecompileHeader.h"
#include "ThreadLoadingLevel.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include "TestStageLevel.h"
#include "FXData.h"
#include "NeroLoading.h"
#include "FadeEffect.h"
#include "Location11_Level.h"
std::string ThreadLoadingLevel::NextLevelName;
ThreadLoadingLevel* ThreadLoadingLevel::Inst = nullptr;

ThreadLoadingLevel::ThreadLoadingLevel()
{
	Inst = this;
}

ThreadLoadingLevel::~ThreadLoadingLevel()
{
	Inst = nullptr;
}


void ThreadLoadingLevel::Start()
{
	BaseLevel::Start();
	GetCamera(0)->SetProjectionType(CameraType::Perspective);

	LoadingUI = CreateActor<NeroLoading>();
	
	std::shared_ptr<FadeEffect> Fade = GetLastTarget()->CreateEffect<FadeEffect>();

	//스레드 로딩이 잘 작동되는지 확인하려는 임시 엑터입니다. 나중에 삭제할 예정이에요
	//ThreadTestActor = CreateActor<GameEngineActor>();
	//std::shared_ptr<GameEngineRenderer> TestRenderer = nullptr;
	//TestRenderer = ThreadTestActor->CreateComponent<GameEngineRenderer>();
	//TestRenderer->CreateRenderUnit("sphere", "MeshTexture");
	//TestRenderer->GetTransform()->SetLocalScale({ 100.0f, 100.0f, 100.0f });
	//ThreadTestActor->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 100.0f });
	//TestRenderer->GetRenderBaseValueRef().IsNormal = 1;


	//===============================================사용법 & 예시=================================================

	// 해당 위치의 지정된 확장자파일들을 모두 로딩합니다
		//PushAllLoadCallBack<어떤 레벨로 넘어갈 때, 어떤 타입의 리소스인지>("위치", { "확장자" });
	// 예시
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character", { ".png", ".tga" });

	//해당 경로의 리소스를 로딩합니다
		//PushLoadCallBack<어떤 레벨로 넘어갈 때, 어떤 타입의 리소스인지>("경로");	
	// 예시
		//PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");

	//===========================================================================================================

	// 통합
	{
		// 플레이어
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Overture\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Wire", { ".fbx" });
		PushLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\High\\pl0000_03_atos.texout.png");
		PushLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\High\\pl0010_01_wing_atos1.texout.png");
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Player\\Nero\\Animation", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Player\\Nero\\Overture\\Animation", { ".fbx" });

		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Player\\Vergil\\Mesh", { ".fbx" });
		PushLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Player\\Vergil\\Mesh\\High\\pl0300_03_atos.texout.png");
		PushLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Player\\Vergil\\Mesh\\High\\pl0310_trans_atos.texout.png");
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Player\\Vergil\\MirageBlade", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Player\\Vergil\\Animation", { ".fbx" });

		// 맵
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\ALPHA_l02_01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_01_p0.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_01_p1.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_02_p0.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_03_p0.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_03_p1.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_04_p0.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_07.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_floor.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_SkyBox.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_wall.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_02.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_03.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_04.fbx");

		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0003_wall_light.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0012_arcadegate01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0026_streetlight01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0039_arcadecaferoof01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0039_arcadecaferoof01_01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0041_fountain01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0044_bench01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0071_electronicbillboard00.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0139_statueb.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0140_streetstall.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0183_decolight00.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0994_statue.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm7001_seal_02.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\LOD_1_Group_0_Sub_3__m02m_illminary01.fbx");
		PushLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_03StreetLine.fbx");

		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_12.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_13.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_14.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_15.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_16.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_20.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_floor.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_SkyBox.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\l11_wall.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\sm0028_storewhite01.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\sm0038_storewood01.fbx");

		PushAllLoadCallBack<Location11_Level, GameEngineFBXMesh>("Mesh\\UIMesh\\ResultMesh", { ".fbx" });
		PushAllLoadCallBack<Location11_Level, GameEngineTexture>("Texture\\UI\\ResultLevel", { ".tga", ".png"});

		//PushAllLoadCallBack<TestStageLevel, GameEngineTexture>("Map\\Location2", { ".tga", ".png"});

		// 텍스쳐
		// 텍스쳐 현재 High로 로드하는데 추후에 추가로 옵션에 따라 로드되게 바꿔주세요
		
		PushAllLoadCallBack<TestStageLevel, GameEngineTexture>("Texture\\UI", { ".tga", ".png"});
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Mesh\\UIMesh", { ".fbx" });

		//PushAllLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Player\\Vergil\\Mesh\\High", { ".tga", ".png"});
		//PushAllLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Enemy\\Empusa\\Mesh\\High", { ".tga", ".png"});
		//PushAllLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Enemy\\HellCaina\\Mesh\\High", { ".tga", ".png"});
		//PushAllLoadCallBack<TestStageLevel, GameEngineTexture>("Character\\Enemy\\Qliphoth\\Mesh\\High", { ".tga", ".png"});

		// 이펙트
		PushAllLoadCallBack<TestStageLevel, FXData>("Effect", { ".effect" });

		// 사운드
		PushAllLoadCallBack<TestStageLevel, GameEngineSound>("Sound", { ".wav", ".ogg"});

		/////////////////// 몬스터 
		// 엠푸사
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Empusa\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Empusa\\Animation", { ".fbx" });
		// 헬카이나
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\HellCaina\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\HellCaina\\Animation", { ".fbx" });
		// 클리포드
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Qliphoth\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Qliphoth\\Animation", { ".fbx" });
		// 헬안테노라
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\HellAntenora\\Mesh", { ".fbx" });
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\HellAntenora\\Animation", { ".fbx" });
		// 스쿠토안젤로
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Scudo\\Mesh", { ".fbx" });
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Scudo\\Animation", { ".fbx" });
		// 프로토안젤로
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Proto\\Mesh", { ".fbx" });
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Proto\\Animation", { ".fbx" });
		// 카발리에안젤로(보스)
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\CavaliereAngelo\\Mesh", { ".fbx" });
		//PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\CavaliereAngelo\\Animation", { ".fbx" });
	}

	// 공용
	{
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\BGM", { ".ogg" });
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\SFX\\Player", { ".wav" });
		//PushAllLoadCallBack<NetworkTestLevel, FXData>("Effect\\Damage", { ".effect" });
	}

	//NetwortTestLevel(레벨별로 구분시켜주세요)
	{
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Effect\\Mesh", { ".fbx" });
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Effect\\Texture", { ".png" });
		//PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
		//PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
		//PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");
	}


}



void ThreadLoadingLevel::InitResource(void* _Ptr)
{
	GameEngineResourceBase* ResPtr = reinterpret_cast<GameEngineResourceBase*>(_Ptr);
	ResPtr->Initialize();
}



void ThreadLoadingLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	LoadingUI->SetThreedPersent(LoadingPercent);
	if (true == IsLoadLevel) 
		return;


	//모든 로딩이 완료된 순간
	if (LoadWorkCount == ExcuteWorkCount)
	{
		LoadingUI->SetThreedPersent(1.f);
		FadeEffect::GetFadeEffect()->FadeOut(0.8f);

		IsLoadLevel = true;
		TimeEvent.AddEvent(1.0f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _TimeEvent)
		{
			GameEngineCore::ChangeLevel(NextLevelName);
		});
		return;
	}
}


void ThreadLoadingLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

	IsLoadLevel = false;
	FadeEffect::GetFadeEffect()->FadeIn(0.8f);
	ThreadLoadStart();
}

void ThreadLoadingLevel::ThreadLoadStart()
{
	const std::vector<std::function<void()>>& CallBacks = AllThreadLoadCallBack[NextLevelName];
	LoadWorkCount = CallBacks.size();
	ExcuteWorkCount = 0;
	LoadingPercent = 0.f;

	if (true == CallBacks.empty())
	{
		LoadWorkCount = 1;
		ExcuteWorkCount = 1;
		LoadingPercent = 1.f;
		return;
	}

	for (const std::function<void()>& CallBack : CallBacks)
	{
		GameEngineCore::JobQueue.Work([this, CallBack](GameEngineThread*)
			{
				CallBack();
				++ExcuteWorkCount;
				LoadingPercent = static_cast<float>(ExcuteWorkCount) / static_cast<float>(LoadWorkCount);
			});
	}
}

