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
#include "Location19_Level.h"
#include "ResultLevel.h"
std::string ThreadLoadingLevel::NextLevelName;
std::string ThreadLoadingLevel::PrevLevelName;
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
		////Location2
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

		PushAllLoadCallBack<TestStageLevel, GameEngineTexture>("Texture\\UI", { ".tga", ".png" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Mesh\\UIMesh", { ".fbx" });

		////Location11
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
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\sm0536_toplight.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\sm0680_floatingisland01.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\sm0680_floatingisland01_01.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\sm0680_floatingisland01_02.fbx");
		PushLoadCallBack<Location11_Level, GameEngineFBXMesh>("Map\\Location2\\sm0026_streetlight01.fbx");

		PushAllLoadCallBack<Location11_Level, GameEngineFBXMesh>("Mesh\\UIMesh\\ResultMesh", { ".fbx" });
		PushAllLoadCallBack<Location11_Level, GameEngineTexture>("Texture\\UI\\ResultLevel", { ".tga", ".png" });

		////Location19
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_01_start.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_floor.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_SkyBox.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_wall.fbx");

		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm0666_qliphothl19parts_tentacleb.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm0667_qliphothl19parts_tentaclec.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm0669_qliphothl19parts_tentaclee.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1369_qliphothpillara.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1370_qliphothpillara_blade.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1371_qliphothpillarb.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1372_qliphothpillarb_under.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1456_qliphoth_cliffa.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1457_qliphoth_cliffb.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1570_clouds00.fbx");
		PushLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1641_spike.fbx");

		PushAllLoadCallBack<Location19_Level, GameEngineFBXMesh>("Mesh\\UIMesh\\ResultMesh", { ".fbx" });
		PushAllLoadCallBack<Location19_Level, GameEngineTexture>("Texture\\UI\\ResultLevel", { ".tga", ".png" });

		// 이펙트
		PushAllLoadCallBack<TestStageLevel, FXData>("Effect", { ".effect" });

		// 사운드
		PushAllLoadCallBack<TestStageLevel, GameEngineSound>("Sound", { ".wav", ".ogg" });

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
		PushAllLoadCallBack<Location19_Level, GameEngineFBXMesh>("Character\\Enemy\\HellAntenora\\Mesh", { ".fbx" });
		PushAllLoadCallBack<Location19_Level, GameEngineFBXAnimation>("Character\\Enemy\\HellAntenora\\Animation", { ".fbx" });
		// 스쿠토안젤로
		PushAllLoadCallBack<Location19_Level, GameEngineFBXMesh>("Character\\Enemy\\Scudo\\Mesh", { ".fbx" });
		PushAllLoadCallBack<Location19_Level, GameEngineFBXAnimation>("Character\\Enemy\\Scudo\\Animation", { ".fbx" });
		// 프로토안젤로
		PushAllLoadCallBack<Location19_Level, GameEngineFBXMesh>("Character\\Enemy\\Proto\\Mesh", { ".fbx" });
		PushAllLoadCallBack<Location19_Level, GameEngineFBXAnimation>("Character\\Enemy\\Proto\\Animation", { ".fbx" });
		// 카발리에안젤로(보스)
		PushAllLoadCallBack<Location11_Level, GameEngineFBXMesh>("Character\\Enemy\\CavaliereAngelo\\Mesh", { ".fbx" });
		PushAllLoadCallBack<Location11_Level, GameEngineFBXAnimation>("Character\\Enemy\\CavaliereAngelo\\Animation", { ".fbx" });
	}

	// 언로드
	{

		// 2스테이지 맵
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\ALPHA_l02_01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_01_p0.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_01_p1.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_02_p0.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_03_p0.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_03_p1.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_04_p0.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_07.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_floor.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_SkyBox.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_wall.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_02.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_03.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\WATER_l02_04.fbx");

		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0003_wall_light.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0012_arcadegate01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0026_streetlight01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0039_arcadecaferoof01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0039_arcadecaferoof01_01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0041_fountain01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0044_bench01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0071_electronicbillboard00.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0139_statueb.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0140_streetstall.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0183_decolight00.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm0994_statue.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\sm7001_seal_02.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\LOD_1_Group_0_Sub_3__m02m_illminary01.fbx");
		PushUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Map\\Location2\\l02_03StreetLine.fbx");

		// 2스테이지 몬스터
		PushAllUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Empusa\\Mesh", { ".fbx" });
		PushAllUnLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Empusa\\Animation", { ".fbx" });
		PushAllUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\HellCaina\\Mesh", { ".fbx" });
		PushAllUnLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\HellCaina\\Animation", { ".fbx" });
		PushAllUnLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Qliphoth\\Mesh", { ".fbx" });
		PushAllUnLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Qliphoth\\Animation", { ".fbx" });


		// 19스테이지 언로드
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_01_start.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_floor.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_SkyBox.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\l19_wall.fbx");

		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm0666_qliphothl19parts_tentacleb.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm0667_qliphothl19parts_tentaclec.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm0669_qliphothl19parts_tentaclee.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1369_qliphothpillara.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1370_qliphothpillara_blade.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1371_qliphothpillarb.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1372_qliphothpillarb_under.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1456_qliphoth_cliffa.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1457_qliphoth_cliffb.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1570_clouds00.fbx");
		PushUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Map\\Location2\\sm1641_spike.fbx");
		
		// 19스테이지 몬스터
		// 헬안테노라
		PushAllUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Character\\Enemy\\HellAntenora\\Mesh", { ".fbx" });
		PushAllUnLoadCallBack<Location19_Level, GameEngineFBXAnimation>("Character\\Enemy\\HellAntenora\\Animation", { ".fbx" });
		// 스쿠토안젤로
		PushAllUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Character\\Enemy\\Scudo\\Mesh", { ".fbx" });
		PushAllUnLoadCallBack<Location19_Level, GameEngineFBXAnimation>("Character\\Enemy\\Scudo\\Animation", { ".fbx" });
		// 프로토안젤로
		PushAllUnLoadCallBack<Location19_Level, GameEngineFBXMesh>("Character\\Enemy\\Proto\\Mesh", { ".fbx" });
		PushAllUnLoadCallBack<Location19_Level, GameEngineFBXAnimation>("Character\\Enemy\\Proto\\Animation", { ".fbx" });


		// 11스테이지 맵 언로드
		// 마지막 스테이지라 언로드 할 필요 없겠지만 일단 언로드한다면 ResultLevel에서 하면 될듯
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_12.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_13.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_14.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_15.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_16.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_20.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_floor.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_SkyBox.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\l11_wall.fbx");

		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\sm0028_storewhite01.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\sm0038_storewood01.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\sm0536_toplight.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\sm0680_floatingisland01.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\sm0680_floatingisland01_01.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\sm0680_floatingisland01_02.fbx");
		//PushUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Map\\Location2\\sm0026_streetlight01.fbx");

		//// 카발리에안젤로(보스)
		//PushAllUnLoadCallBack<ResultLevel, GameEngineFBXMesh>("Character\\Enemy\\CavaliereAngelo\\Mesh", { ".fbx" });
		//PushAllUnLoadCallBack<ResultLevel, GameEngineFBXAnimation>("Character\\Enemy\\CavaliereAngelo\\Animation", { ".fbx" });

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

	// 언로드가 끝난 순간
	if (true == IsUnload && UnLoadWorkCount == ExcuteWorkCount)
	{
		StartLoad();
	}
	// 언로드 밑 로드가 끝난 순간
	if (true == IsLoad && AllWorkCount == ExcuteWorkCount)
	{
		LoadingUI->SetThreedPersent(1.f);
		GameEngineCore::ChangeLevel(NextLevelName);
		return;
	}
}


void ThreadLoadingLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

	ThreadLoadStart();
}

void ThreadLoadingLevel::StartUnLoad()
{
	IsUnload = true;
	IsLoad = false;
	const std::vector<std::function<void()>>& CallBacks = AllThreadUnLoadCallBack[PrevLevelName];

	for (const std::function<void()>& CallBack : CallBacks)
	{
		GameEngineCore::JobQueue.Work([this, CallBack](GameEngineThread*)
			{
				CallBack();
				++ExcuteWorkCount;
				LoadingPercent = static_cast<float>(ExcuteWorkCount) / static_cast<float>(AllWorkCount);
			});
	}
}

void ThreadLoadingLevel::StartLoad()
{
	IsUnload = false;
	IsLoad = true;
	const std::vector<std::function<void()>>& CallBacks = AllThreadLoadCallBack[NextLevelName];

	for (const std::function<void()>& CallBack : CallBacks)
	{
		GameEngineCore::JobQueue.Work([this, CallBack](GameEngineThread*)
			{
				CallBack();
				++ExcuteWorkCount;
				LoadingPercent = static_cast<float>(ExcuteWorkCount) / static_cast<float>(AllWorkCount);
			});
	}
}

void ThreadLoadingLevel::ThreadLoadStart()
{
	const std::vector<std::function<void()>>& UnLoadCallBacks = AllThreadUnLoadCallBack[PrevLevelName];
	ExcuteWorkCount = 0;
	LoadingPercent = 0.f;
	if (UnLoadCallBacks.empty())
	{
		UnLoadWorkCount = 1;
		ExcuteWorkCount += 1;
	}
	else
	{
		UnLoadWorkCount = UnLoadCallBacks.size();
	}

	const std::vector<std::function<void()>>& LoadCallBacks = AllThreadLoadCallBack[NextLevelName];
	if (LoadCallBacks.empty())
	{
		LoadWorkCount = 1;
		ExcuteWorkCount += 1;
	}
	else
	{
		LoadWorkCount = LoadCallBacks.size();
	}

	AllWorkCount = UnLoadWorkCount + LoadWorkCount;

	StartUnLoad();
}

