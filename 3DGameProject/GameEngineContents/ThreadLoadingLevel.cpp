#include "PrecompileHeader.h"
#include "ThreadLoadingLevel.h"

#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include "TestStageLevel.h"
#include "FXData.h"
#include "NeroLoading.h"

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


	//������ �ε��� �� �۵��Ǵ��� Ȯ���Ϸ��� �ӽ� �����Դϴ�. ���߿� ������ �����̿���
	//ThreadTestActor = CreateActor<GameEngineActor>();
	//std::shared_ptr<GameEngineRenderer> TestRenderer = nullptr;
	//TestRenderer = ThreadTestActor->CreateComponent<GameEngineRenderer>();
	//TestRenderer->CreateRenderUnit("sphere", "MeshTexture");
	//TestRenderer->GetTransform()->SetLocalScale({ 100.0f, 100.0f, 100.0f });
	//ThreadTestActor->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 100.0f });
	//TestRenderer->GetRenderBaseValueRef().IsNormal = 1;


	//===============================================���� & ����=================================================

	// �ش� ��ġ�� ������ Ȯ�������ϵ��� ��� �ε��մϴ�
		//PushAllLoadCallBack<� ������ �Ѿ ��, � Ÿ���� ���ҽ�����>("��ġ", { "Ȯ����" });
	// ����
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character", { ".png", ".tga" });

	//�ش� ����� ���ҽ��� �ε��մϴ�
		//PushLoadCallBack<� ������ �Ѿ ��, � Ÿ���� ���ҽ�����>("���");	
	// ����
		//PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");

	//===========================================================================================================

	// ����
	{
		// �÷��̾�
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
		
		// ��

		// ����Ʈ
		PushAllLoadCallBack<TestStageLevel, FXData>("Effect", { ".effect" });

		// ����
		PushAllLoadCallBack<TestStageLevel, GameEngineSound>("Sound", { ".wav", ".ogg"});

		// ���� 
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Empusa\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Empusa\\Animation", { ".fbx" });

		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\HellCaina\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\HellCaina\\Animation", { ".fbx" });

		PushAllLoadCallBack<TestStageLevel, GameEngineFBXMesh>("Character\\Enemy\\Qliphoth\\Mesh", { ".fbx" });
		PushAllLoadCallBack<TestStageLevel, GameEngineFBXAnimation>("Character\\Enemy\\Qliphoth\\Animation", { ".fbx" });
	}
	
	//// �׷�
	//{
	//	PushAllLoadCallBack<NetworkTestLevel, FXData>("Effect\\Nero", { ".effect" });
	//	PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
	//	PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Overture\\Mesh\\Overture.fbx");
	//	PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Wire\\Nero_WireArm.fbx");
	//	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");
	//	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
	//	PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXAnimation>("Character\\Player\\Nero\\Animation", { ".fbx" });
	//	PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXAnimation>("Character\\Player\\Nero\\Overture\\Animation", { ".fbx" });
	//	PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\SFX\\Nero", { ".wav" });
	//	PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\Voice\\Nero", { ".wav" });
	//}

	//// ����
	//{
	//	PushAllLoadCallBack<NetworkTestLevel, FXData>("Effect\\Vergil", { ".effect" });
	//	PushAllLoadCallBack<NetworkTestLevel, FXData>("Effect\\Mirage", { ".effect" });
	//	PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Vergil\\Mesh\\Vergil.fbx");
	//	PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Vergil\\Mesh\\pl0300_03_atos.texout.png");
	//	PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXAnimation>("Character\\Player\\Vergil\\Animation", { ".fbx" });
	//	PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\SFX\\Vergil", { ".wav" });
	//	PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\Voice\\Vergil", { ".wav" });
	//}


	// ����
	{
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\BGM", { ".ogg" });
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineSound>("Sound\\SFX\\Player", { ".wav" });
		//PushAllLoadCallBack<NetworkTestLevel, FXData>("Effect\\Damage", { ".effect" });
	}

	//NetwortTestLevel(�������� ���н����ּ���)
	{
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Effect\\Mesh", { ".fbx" });
		//PushAllLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Effect\\Texture", { ".png" });
		//PushLoadCallBack<NetworkTestLevel, GameEngineFBXMesh>("Character\\Player\\Nero\\Mesh\\Nero.fbx");
		//PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0010_03_atos.texout.png");
		//PushLoadCallBack<NetworkTestLevel, GameEngineTexture>("Character\\Player\\Nero\\Mesh\\pl0000_03_atos.texout.png");
	}


}

void ThreadLoadingLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

	//��� �ε��� �Ϸ�� ����
	if (LoadWorkCount == ExcuteWorkCount)
	{
		LoadingUI->SetThreedPersent(1.f);
		GameEngineCore::ChangeLevel(NextLevelName);
		return;
	}

	LoadingUI->SetThreedPersent(LoadingPercent);
}


void ThreadLoadingLevel::LevelChangeStart()
{
	BaseLevel::LevelChangeStart();

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

