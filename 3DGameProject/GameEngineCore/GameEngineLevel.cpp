#include "PrecompileHeader.h"
#include "GameEngineLevel.h"

#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineCore/GameEngineLight.h>

#include "GameEngineActor.h"
#include "GameEngineCamera.h"
#include "GameEngineGUI.h"
#include "GameEngineCollision.h"
#include "GameEngineDebug3D.h"
#include "GameEngineLight.h"
#include "GameEnginePhysics.h"
#include "GameEngineDevice.h"

bool GameEngineLevel::IsDebugRender = false;

GameEngineLevel::GameEngineLevel()
{
	LevelCameraInit();
	LevelLightInit();
}

GameEngineLevel::~GameEngineLevel()
{

}

void GameEngineLevel::CameraBasalInit()
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupStartIter = Cameras.begin();
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupEndIter = Cameras.end();

	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
	{
		std::shared_ptr<GameEngineCamera> InitCamera = GroupStartIter->second;
		InitCamera->CameraBasalAdd();
	}
}

void GameEngineLevel::LevelCameraInit()
{
	MainCamera = CreateNewCamera(0);
	MainCamera->SetName("Main Camera");

	std::shared_ptr<GameEngineCamera> UICamera = CreateNewCamera(100);
	UICamera->SetProjectionType(CameraType::Orthogonal);
	UICamera->SetName("UI Camera");

	if (nullptr == LastTarget)
	{
		LastTarget = GameEngineRenderTarget::Create();
		LastTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}
	if (nullptr == ScreenShootTarget)
	{
		ScreenShootTarget = GameEngineRenderTarget::Create();
		ScreenShootTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}
}

void GameEngineLevel::LevelLightInit()
{
	if (nullptr == DirectionalLight)
	{
		DirectionalLight = CreateActor<GameEngineLight>();
		DirectionalLight->SetName("Directional Light");
		DirectionalLight->SetLightType(LightType::Directional);
		DirectionalLight->LightDataValue.LightPower = 1.0f;
	}
}

std::shared_ptr<GameEngineLight> GameEngineLevel::CreatePointLight(const float4& _Pos, float _Range)
{
	std::shared_ptr<GameEngineLight> NewLight = CreateActor<GameEngineLight>();

	NewLight->GetTransform()->SetLocalPosition(_Pos);

	NewLight->SetName("Point Light");
	NewLight->SetLightType(LightType::Point);

	NewLight->LightDataValue.LightRange = _Range;

	return NewLight;
}

std::shared_ptr<GameEngineLight> GameEngineLevel::CreateSpotLight(const float4& _Pos, float _Range, float _Angle)
{
	std::shared_ptr<GameEngineLight> NewLight = CreateActor<GameEngineLight>();

	NewLight->GetTransform()->SetLocalPosition(_Pos);

	NewLight->SetName("Spot Light");
	NewLight->SetLightType(LightType::Spot);

	NewLight->LightDataValue.LightRange = _Range;
	NewLight->LightDataValue.LightAngle = _Angle;

	return NewLight;
}

void GameEngineLevel::ActorSetName(std::shared_ptr<GameEngineActor> _Actor, std::string _Name)
{
	_Actor->SetName(_Name);
}

void GameEngineLevel::Start()
{
}

void GameEngineLevel::Update(float _DeltaTime)
{

}

void GameEngineLevel::Render(float _DeltaTime)
{
	if (true == IsDebugRender)
	{
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupStartIter = Collisions.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupEndIter = Collisions.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineCollision>>& ObjectList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectStart = ObjectList.begin();
			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectEnd = ObjectList.end();

			for (; ObjectStart != ObjectEnd; ++ObjectStart)
			{
				std::shared_ptr<GameEngineCollision> CollisionObject = (*ObjectStart);

				if (nullptr == CollisionObject)
				{
					continue;
				}

				if (false == CollisionObject->IsUpdate())
				{
					continue;
				}

				CollisionObject->DebugRender(_DeltaTime);
			}
		}
	}

	for (std::pair<int, std::shared_ptr<GameEngineCamera>> Pair : Cameras)
	{

		std::shared_ptr<GameEngineCamera> Cam = Pair.second;

		LightDataObject.LightCount = 0;
		for (std::shared_ptr<GameEngineLight> Light : AllLight)
		{
			Light->LightUpdate(Cam.get(), _DeltaTime);
			LightDataObject.AllLight[LightDataObject.LightCount] = Light->GetLightData();
			++LightDataObject.LightCount;
		}

		Cam->ViewPortSetting();
		Cam->Setting();

		Cam->CameraTransformUpdate();
		Cam->Render(_DeltaTime);

		Cam->ForwardMerge(_DeltaTime);
		Cam->DeferredMerge(_DeltaTime);

		if (false == IsDebugRender)
		{
			continue;
		}

		GameEngineDebug::DebugRender(Cam.get(), _DeltaTime);
	}

	LastTarget->Clear();

	for (std::pair<int, std::shared_ptr<GameEngineCamera>> Pair : Cameras)
	{
		std::shared_ptr<GameEngineCamera> Camera = Pair.second;
		std::shared_ptr<GameEngineRenderTarget> Target = Camera->CamTarget;

		LastTarget->Merge(Target);

		if (MainCamera == Camera)
		{
			ScreenShootTarget->Clear();
			ScreenShootTarget->Merge(Target);
			ScreenShootTarget->Setting();
		}
	}

	LastTarget->Effect(_DeltaTime);

	LastTarget->Setting();

	GameEngineDevice::GetBackBufferTarget()->Merge(LastTarget);

	static bool GUIRender = true;

	if (true == GameEngineInput::IsDown("GUISwitch"))
	{
		GUIRender = !GUIRender;

		if (false == GUIRender)
		{
			GameEngineGUI::Release();
		}
		else {
			GameEngineGUI::Initialize();
		}
	}

	if (true == GUIRender)
	{
		GameEngineGUI::Render(GetSharedThis(), _DeltaTime);
	}
}

void GameEngineLevel::LevelChangeStart()
{
}
void GameEngineLevel::LevelChangeEnd()
{
}

void GameEngineLevel::ActorUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("FreeCameraSwitch"))
	{
		MainCamera->FreeCameraSwitch();
		// GameEngineInput::CreateKey("FreeCameraSwitch", VK_F1);
	}

	if (true == MainCamera->IsFreeCamera())
	{
		GameEnginePhysics::SetIsPhysics(true);
		MainCamera->Update(_DeltaTime);
		GameEngineInput::AllReset();
	}
	else
	{
		GameEnginePhysics::SetIsPhysics(false);
	}

	{
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			float ScaleTime = _DeltaTime * GameEngineTime::GlobalTime.GetUpdateOrderTimeScale(GroupStartIter->first);

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;

				Actor->AllAccTime(ScaleTime);
				Actor->AllUpdate(ScaleTime);
			}
		}
	}
}

void GameEngineLevel::ActorLevelChangeStart()
{
	{
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;

				Actor->AllLevelChangeStart();
			}
		}
	}
}

void GameEngineLevel::ActorLevelChangeEnd()
{
	{
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;

				Actor->AllLevelChangeEnd();
			}
		}
	}
}

void GameEngineLevel::PostProcessLevelChangeStart()
{
	std::vector<std::shared_ptr<GameEnginePostProcess>>& LastTargetProcess = LastTarget->Effects;

	for (size_t i = 0; i < LastTargetProcess.size(); i++)
	{
		LastTargetProcess[i]->LevelChangeStart();
	}

	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator CameraLoop = Cameras.begin();
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator CameraEnd = Cameras.end();

	while (CameraLoop != CameraEnd)
	{
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamEffectLoop = CameraLoop->second->CamTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamEffectEnd = CameraLoop->second->CamTarget->Effects.end();

		while (CamEffectLoop != CamEffectEnd)
		{
			(*CamEffectLoop)->LevelChangeStart();

			++CamEffectLoop;
		}

		++CameraLoop;
	}
}

void GameEngineLevel::PostProcessLevelChangeEnd()
{
	std::vector<std::shared_ptr<GameEnginePostProcess>>& LastTargetProcess = LastTarget->Effects;

	for (size_t i = 0; i < LastTargetProcess.size(); i++)
	{
		LastTargetProcess[i]->LevelChangeEnd();
	}

	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator CameraLoop = Cameras.begin();
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator CameraEnd = Cameras.end();

	while (CameraLoop != CameraEnd)
	{
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamEffectLoop = CameraLoop->second->CamTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamEffectEnd = CameraLoop->second->CamTarget->Effects.end();

		while (CamEffectLoop != CamEffectEnd)
		{
			(*CamEffectLoop)->LevelChangeEnd();
			++CamEffectLoop;
		}

		++CameraLoop;
	}
}

void GameEngineLevel::RenderTargetTextureRelease()
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupStartIter = Cameras.begin();
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupEndIter = Cameras.end();

	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
	{
		std::shared_ptr<GameEngineCamera> InitCamera = GroupStartIter->second;
		InitCamera->CameraRenderTargetRelease();
	}
}

void GameEngineLevel::ActorRelease()
{
	for (std::pair<int, std::shared_ptr<GameEngineCamera>> Pair : Cameras)
	{
		std::shared_ptr<GameEngineCamera> Cam = Pair.second;
		Cam->Release();
	}

	{
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupStartIter = Collisions.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineCollision>>>::iterator GroupEndIter = Collisions.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineCollision>>& ObjectList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectStart = ObjectList.begin();
			std::list<std::shared_ptr<GameEngineCollision>>::iterator ObjectEnd = ObjectList.end();

			for (; ObjectStart != ObjectEnd; )
			{
				std::shared_ptr<GameEngineCollision> RelaseObject = (*ObjectStart);

				if (nullptr != RelaseObject && false == RelaseObject->IsDeath())
				{
					++ObjectStart;
					continue;
				}

				ObjectStart = ObjectList.erase(ObjectStart);
			}
		}
	}

	{
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; )
			{
				std::shared_ptr<GameEngineActor> RelaseActor = (*ActorStart);

				RelaseActor->AllDestroy();

				if (nullptr != RelaseActor && false == RelaseActor->IsDeath())
				{
					RelaseActor->AllRelease();
					++ActorStart;
					continue;
				}

				RelaseActor->Release();
				ActorStart = ActorList.erase(ActorStart);
			}
		}
	}
}

void GameEngineLevel::ActorInit(std::shared_ptr<GameEngineActor> _Actor, int _Order, GameEngineLevel* _Level)
{
	_Actor->Level = this;
	_Actor->SetOrder(_Order);
	_Actor->Start();
}

void GameEngineLevel::PushCollision(std::shared_ptr<GameEngineCollision> _Collision)
{
	Collisions[_Collision->GetOrder()].push_back(_Collision);
}

std::shared_ptr<GameEngineCamera> GameEngineLevel::CreateNewCamera(int _Order)
{
	if (Cameras.find(_Order) != Cameras.end())
	{
		MsgAssert("이미 존재하는 오더의 카메라를 중복 생성하려했습니다 Order : " + std::to_string(_Order));
		return nullptr;
	}

	std::shared_ptr<GameEngineCamera> NewCamera = CreateActor<GameEngineCamera>();
	Cameras.insert(std::make_pair(_Order, NewCamera));

	return NewCamera;
}

void GameEngineLevel::PushCameraRenderer(std::shared_ptr<GameEngineRenderer> _Renderer, int _CameraOrder)
{
	std::shared_ptr<GameEngineCamera> FindCamera = GetCamera(_CameraOrder);

	if (nullptr == FindCamera)
	{
		MsgAssert("존재하지 않는 카메라에 랜더러를 넣을수는 없습니다.");
		return;
	}

	FindCamera->PushRenderer(_Renderer);
}

std::shared_ptr<GameEngineCamera> GameEngineLevel::GetCamera(int _CameraOrder)
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator FindIter = Cameras.find(_CameraOrder);

	if (FindIter == Cameras.end())
	{
		return nullptr;
	}

	std::shared_ptr<GameEngineCamera> Camera = FindIter->second;

	return Camera;
}

void GameEngineLevel::TextureUnLoad(GameEngineLevel* _NextLevel)
{
	for (const std::pair<std::string, std::string>& Pair : LoadEndPath)
	{
		if (nullptr != _NextLevel && true == _NextLevel->TexturePath.contains(Pair.first))
		{
			continue;
		}

		GameEngineTexture::UnLoad(Pair.first);
		TexturePath.insert(std::make_pair(Pair.first, Pair.second));
	}

	LoadEndPath.clear();
}

void GameEngineLevel::TextureReLoad(GameEngineLevel* _PrevLevel)
{
	for (const std::pair<std::string, std::string>& Pair : TexturePath)
	{
		if (nullptr != _PrevLevel && true == _PrevLevel->TexturePath.contains(Pair.first))
		{
			continue;
		}

		GameEngineTexture::ReLoad(Pair.second, Pair.first);
		LoadEndPath.insert(std::make_pair(Pair.first, Pair.second));
	}

	TexturePath.clear();
}

void GameEngineLevel::AllActorDestroy()
{
	DestroyCamera();
	{
		// 이건 나중에 만들어질 랜더러의 랜더가 다 끝나고 되는 랜더가 될겁니다.
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupStartIter = Actors.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator GroupEndIter = Actors.end();

		for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineActor>>& ActorList = GroupStartIter->second;

			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStart = ActorList.begin();
			std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEnd = ActorList.end();

			for (; ActorStart != ActorEnd; ++ActorStart)
			{
				std::shared_ptr<GameEngineActor>& Actor = *ActorStart;
				Actor->Death();
			}
		}

		ActorRelease();
	}

	LevelCameraInit();
}

void GameEngineLevel::DestroyCamera()
{
	for (std::pair<int, std::shared_ptr<GameEngineCamera>> _Cam : Cameras)
	{
		_Cam.second->Renderers.clear();
	}
	Cameras.clear();
}

void GameEngineLevel::PushLight(std::shared_ptr<GameEngineLight> _Light)
{
	// 라이트가 지워질때 어떻게 하지?
	//LightData& LightInst = LightDataObject.AllLight[LightDataObject.LightCount];

	//_Light->LightDataPtr = &LightInst;
	//++LightDataObject.LightCount;
	AllLight.push_back(_Light);
}