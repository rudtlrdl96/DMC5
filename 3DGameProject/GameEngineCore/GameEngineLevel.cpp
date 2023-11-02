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

void GameEngineLevel::RenderTargetTextureCreate()
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupStartIter = Cameras.begin();
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupEndIter = Cameras.end();

	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
	{
		std::shared_ptr<GameEngineCamera> InitCamera = GroupStartIter->second;
		InitCamera->RenderTargetTextureLoad();
	}
	for (std::shared_ptr<GameEngineLight> Light : AllLight)
	{
		Light->ShadowTargetTextureLoad(Light->ShadowTextureScaleValue);
	}

	LastTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	ScreenShootTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
}

void GameEngineLevel::RenderTargetTextureRelease()
{
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupStartIter = Cameras.begin();
	std::map<int, std::shared_ptr<GameEngineCamera>>::iterator GroupEndIter = Cameras.end();

	for (; GroupStartIter != GroupEndIter; ++GroupStartIter)
	{
		std::shared_ptr<GameEngineCamera> InitCamera = GroupStartIter->second;
		InitCamera->RenderTargetTextureRelease();
	}
	for (std::shared_ptr<GameEngineLight> Light : AllLight)
	{
		Light->ShadowTargetTextureRelease();
	}

	LastTarget->ReleaseTextures();
	ScreenShootTarget->ReleaseTextures();
}

void GameEngineLevel::LevelCameraInit()
{
	MainCamera = CreateNewCamera(0);
	MainCamera->SetName("Main Camera");
	MainCamera->CreateCamDeferrdTarget();

	std::shared_ptr<GameEngineCamera> UICamera = CreateNewCamera(100);
	UICamera->SetProjectionType(CameraType::Orthogonal);
	UICamera->SetName("UI Camera");

	if (nullptr == LastTarget)
	{
		LastTarget = GameEngineRenderTarget::Create();
	}
	if (nullptr == ScreenShootTarget)
	{
		ScreenShootTarget = GameEngineRenderTarget::Create();
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
		DirectionalLight->IsShadowLight = true;
		DirectionalLight->ShadowTextureScaleValue = ShadowTextureScale::S_16384;
		DirectionalLight->LightDataValue.ShadowTargetSizeX = 16384;
		DirectionalLight->LightDataValue.ShadowTargetSizeY = 16384;
		DirectionalLight->ShadowRange.x = 16384 * 2.0f;
		DirectionalLight->ShadowRange.y = 16384 * 2.0f;
		DirectionalLight->LightDataValue.LightColor = float4(1.0f, 1.0f, 1.0f);
		DirectionalLight->SetSpcLightPower(0.1f);
		DirectionalLight->SetLightRange(20000.0f);
		DirectionalLight->DynamicShadowOn();
	}
}

std::shared_ptr<GameEngineLight> GameEngineLevel::CreatePointLight(const float4& _Pos, ShadowTextureScale _ShadowScale, float _Range)
{
	std::shared_ptr<GameEngineLight> NewLight = CreateActor<GameEngineLight>();
	NewLight->IsShadowLight = true;

	NewLight->GetTransform()->SetLocalPosition(_Pos);
	NewLight->SetName("Point Light");
	NewLight->SetLightType(LightType::Point);

	NewLight->SetLightRange(_Range);
	NewLight->SetLightAngle(360);

	NewLight->ShadowTargetTextureLoad(_ShadowScale);

	return NewLight;
}

std::shared_ptr<GameEngineLight> GameEngineLevel::CreateSpotLight(const float4& _Pos, ShadowTextureScale _ShadowScale, float _Range, float _Angle)
{
	std::shared_ptr<GameEngineLight> NewLight = CreateActor<GameEngineLight>();
	NewLight->IsShadowLight = true;

	NewLight->GetTransform()->SetLocalPosition(_Pos);
	NewLight->SetName("Spot Light");
	NewLight->SetLightType(LightType::Spot);

	NewLight->SetLightRange(_Range);
	NewLight->SetLightAngle(_Angle);

	NewLight->ShadowTargetTextureLoad(_ShadowScale);
	 
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
	for (std::shared_ptr<GameEngineLight> Light : AllLight)
	{
		if (false == Light->IsShadow())
		{
			continue;
		}

		std::shared_ptr<GameEngineRenderTarget> ShadowTarget = Light->GetShadowTarget();
		std::shared_ptr<GameEngineRenderTarget> BakeTarget = Light->GetBakeTarget(Light->GetBakeTargetIndex());

		if (nullptr != ShadowTarget)
		{
			ShadowTarget->Clear();

			if (Light->GetLightData().LightType == static_cast<int>(LightType::Point))
			{
				ShadowTarget->MergeCubemap(BakeTarget);
			}
			else
			{
				ShadowTarget->Merge(BakeTarget);
			}
		}
	}

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

		Cam->CameraTransformUpdate();
		Cam->ViewPortSetting();

		LightDataObject.LightCount = 0;

		for (std::shared_ptr<GameEngineLight> Light : AllLight)
		{
			Light->LightUpdate(Cam.get(), _DeltaTime);
			LightDataObject.AllLight[LightDataObject.LightCount] = Light->GetLightData();
			++LightDataObject.LightCount;
		}

		Cam->Render(_DeltaTime);

		if (false == IsDebugRender)
		{
			GameEngineDebug::ClearRender(Cam.get());
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
	//if (true == GameEngineInput::IsDown("FreeCameraSwitch"))
	//{
	//	// MainCamera->FreeCameraSwitch();
	//	// GameEngineInput::CreateKey("FreeCameraSwitch", VK_F1);
	//}
	if (true == GameEngineInput::IsDown("CollisionDebugSwitch"))
	{
		IsDebugSwitch();
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
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamLastLoop = CameraLoop->second->CamTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamLastEnd = CameraLoop->second->CamTarget->Effects.end();

		while (CamLastLoop != CamLastEnd)
		{
			(*CamLastLoop)->LevelChangeStart();
			++CamLastLoop;
		}

		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamForwardLoop = CameraLoop->second->CamForwardTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamForwardEnd = CameraLoop->second->CamForwardTarget->Effects.end();

		while (CamForwardLoop != CamForwardEnd)
		{
			(*CamForwardLoop)->LevelChangeStart();
			++CamForwardLoop;
		}

		if (nullptr != CameraLoop->second->CamDeferrdTarget)
		{
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferrdLoop = CameraLoop->second->CamDeferrdTarget->Effects.begin();
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferrdEnd = CameraLoop->second->CamDeferrdTarget->Effects.end();

			while (CamDeferrdLoop != CamDeferrdEnd)
			{
				(*CamDeferrdLoop)->LevelChangeStart();
				++CamDeferrdLoop;
			}
		}

		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamAlphaLoop = CameraLoop->second->CamAlphaTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamAlphaEnd = CameraLoop->second->CamAlphaTarget->Effects.end();

		while (CamAlphaLoop != CamAlphaEnd)
		{
			(*CamAlphaLoop)->LevelChangeStart();
			++CamAlphaLoop;
		}		
		
		if (nullptr != CameraLoop->second->DeferredLightTarget)
		{
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferredLightLoop = CameraLoop->second->DeferredLightTarget->Effects.begin();
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferredLightEnd = CameraLoop->second->DeferredLightTarget->Effects.end();

			while (CamDeferredLightLoop != CamDeferredLightEnd)
			{
				(*CamDeferredLightLoop)->LevelChangeStart();
				++CamDeferredLightLoop;
			}
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
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamLastLoop = CameraLoop->second->CamTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamLastEnd = CameraLoop->second->CamTarget->Effects.end();

		while (CamLastLoop != CamLastEnd)
		{
			(*CamLastLoop)->LevelChangeEnd();
			++CamLastLoop;
		}

		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamForwardLoop = CameraLoop->second->CamForwardTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamForwardEnd = CameraLoop->second->CamForwardTarget->Effects.end();

		while (CamForwardLoop != CamForwardEnd)
		{
			(*CamForwardLoop)->LevelChangeEnd();
			++CamForwardLoop;
		}

		if (nullptr != CameraLoop->second->CamDeferrdTarget)
		{
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferrdLoop = CameraLoop->second->CamDeferrdTarget->Effects.begin();
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferrdEnd = CameraLoop->second->CamDeferrdTarget->Effects.end();

			while (CamDeferrdLoop != CamDeferrdEnd)
			{
				(*CamDeferrdLoop)->LevelChangeEnd();
				++CamDeferrdLoop;
			}
		}

		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamAlphaLoop = CameraLoop->second->CamAlphaTarget->Effects.begin();
		std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamAlphaEnd = CameraLoop->second->CamAlphaTarget->Effects.end();

		while (CamAlphaLoop != CamAlphaEnd)
		{
			(*CamAlphaLoop)->LevelChangeEnd();
			++CamAlphaLoop;
		}

		if (nullptr != CameraLoop->second->DeferredLightTarget)
		{
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferredLightLoop = CameraLoop->second->DeferredLightTarget->Effects.begin();
			std::vector<std::shared_ptr<GameEnginePostProcess>>::iterator CamDeferredLightEnd = CameraLoop->second->DeferredLightTarget->Effects.end();

			while (CamDeferredLightLoop != CamDeferredLightEnd)
			{
				(*CamDeferredLightLoop)->LevelChangeEnd();
				++CamDeferredLightLoop;
			}
		}

		++CameraLoop;
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

void GameEngineLevel::LightRelease()
{
	std::vector<std::shared_ptr<GameEngineLight>>::iterator LoopIter = AllLight.begin();
	std::vector<std::shared_ptr<GameEngineLight>>::iterator EndIter = AllLight.end();

	while (LoopIter != EndIter)
	{
		std::shared_ptr<GameEngineLight> LightPtr = *LoopIter;

		if (true == LightPtr->IsDeath())
		{
			LoopIter = AllLight.erase(LoopIter);
			EndIter = AllLight.end();
		}
		else
		{
			++LoopIter;
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