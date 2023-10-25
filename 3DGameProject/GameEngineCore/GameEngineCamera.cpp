#include "PrecompileHeader.h"
#include "GameEngineCamera.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include "GameEngineRenderer.h"
#include "GameEngineDevice.h"
#include "GameEngineRenderer.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineTexture.h"
#include "GameEngineMaterial.h"
#include "GameEnginePixelShader.h"
#include "GameEngineLight.h"

GameEngineCamera::GameEngineCamera()
{
	SetName("GameEngineCamera");
}

GameEngineCamera::~GameEngineCamera()
{
}

void GameEngineCamera::CaptureCubemap(const float4& _Pos, const float4& _Rot, const float4& _CaptureScale /*= float4(128, 128)*/)
{
	TransformData CurTransData = GetTransform()->GetTransDataRef();
	
	GetTransform()->SetWorldPosition(_Pos);
	GetTransform()->SetWorldRotation(_Rot);
	
	float CurWidth = Width;
	float CurHeight = Height;
	
	Width = _CaptureScale.x;
	Height = _CaptureScale.y;

	CamTarget->Clear();
	CamPosTarget->Clear();
	CamForwardTarget->Clear();

	if (nullptr != CamDeferrdTarget)
	{
		CamDeferrdTarget->Clear();
	}

	CamAlphaTarget->Clear();

	for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
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

	CameraTransformUpdate();
	ViewPortSetting();

	// Light
	GetLevel()->LightDataObject.LightCount = 0;
	for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
	{
		Light->LightUpdate(this, 0.0f);
		GetLevel()->LightDataObject.AllLight[GetLevel()->LightDataObject.LightCount] = Light->GetLightData();
		++GetLevel()->LightDataObject.LightCount;
	}

	Render(0.0f);

	Width = CurWidth;
	Height = CurHeight;
	
	GetTransform()->SetTransformData(CurTransData);
}

void GameEngineCamera::CreateCamDeferrdTarget()
{
	if (nullptr == CamDeferrdTarget)
	{
		CamDeferrdTarget = GameEngineRenderTarget::Create();
		DeferredLightTarget = GameEngineRenderTarget::Create();
	}
}

void GameEngineCamera::Start()
{
	if (false == GameEngineInput::IsKey("CamMoveLeft"))
	{
		GameEngineInput::CreateKey("CamMoveLeft", 'A');
		GameEngineInput::CreateKey("CamMoveRight", 'D');
		GameEngineInput::CreateKey("CamMoveUp", 'Q');
		GameEngineInput::CreateKey("CamMoveDown", 'E');
		GameEngineInput::CreateKey("CamMoveForward", 'W');
		GameEngineInput::CreateKey("CamMoveBack", 'S');

		GameEngineInput::CreateKey("RotY+", VK_NUMPAD1);
		GameEngineInput::CreateKey("RotY-", VK_NUMPAD2);
		GameEngineInput::CreateKey("RotZ+", VK_NUMPAD4);
		GameEngineInput::CreateKey("RotZ-", VK_NUMPAD5);
		GameEngineInput::CreateKey("RotX+", VK_NUMPAD7);
		GameEngineInput::CreateKey("RotX-", VK_NUMPAD8);

		GameEngineInput::CreateKey("CamRot", VK_RBUTTON);
		GameEngineInput::CreateKey("SpeedBoost", VK_LSHIFT);
		GameEngineInput::CreateKey("FreeCameraSwitch", VK_F1);
		GameEngineInput::CreateKey("ProjectionModeChange", VK_F2);
		GameEngineInput::CreateKey("CollisionDebugSwitch", VK_F3);
		GameEngineInput::AddIgnoreKeys("FreeCameraSwitch");
	}

	ViewPortData.TopLeftX = 0;
	ViewPortData.TopLeftY = 0;
	ViewPortData.Width = GameEngineWindow::GetScreenSize().x;
	ViewPortData.Height = GameEngineWindow::GetScreenSize().y;
	ViewPortData.MinDepth = 0.0f;
	ViewPortData.MaxDepth = 1.0f;

	// 큐브
	{
		//ViewPortData[0].TopLeftX = 0;
		//ViewPortData[0].TopLeftY = 0;
		//ViewPortData[0].Width = 256; // Texture->Desc.Width
		//ViewPortData[0].Height = 256; // Texture->Desc.Height
		//ViewPortData[0].MinDepth = 0.0f;
		//ViewPortData[0].MaxDepth = 1.0f;
	}

	Width = ViewPortData.Width;
	Height = ViewPortData.Height;

	CamTarget = GameEngineRenderTarget::Create();
	CamPosTarget = GameEngineRenderTarget::Create();

	CamForwardTarget = GameEngineRenderTarget::Create();

	CamAlphaTarget = GameEngineRenderTarget::Create();

	//CubeRenderTarget = GameEngineRenderTarget::Create();
}

void GameEngineCamera::RenderTargetTextureLoad()
{
	if (true == IsLoad)
	{
		return;
	}

	CamTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamPosTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamTarget->CreateDepthTexture();

	CamForwardTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamForwardTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamForwardTarget->SetDepthTexture(CamTarget->GetDepthTexture());

	if (nullptr != CamDeferrdTarget)
	{
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
		CamDeferrdTarget->SetDepthTexture(CamTarget->GetDepthTexture());
	}

	if (nullptr != DeferredLightTarget)
	{
		DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 원래것, 디퓨즈 라이트
		DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 스펙큘러 라이트
		DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 앰비언트 라이트를 담는다.
		DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // 블룸
		//DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // SSS BackLight

		CalLightUnit.SetMesh("FullRect");
		CalLightUnit.SetMaterial("DeferredCalLight");

		LightDatas& Data = GetLevel()->LightDataObject;
		CalLightUnit.ShaderResHelper.SetConstantBufferLink("LightDatas", Data);
		CalLightUnit.ShaderResHelper.SetTexture("PositionTex", CamDeferrdTarget->GetTexture(1));
		CalLightUnit.ShaderResHelper.SetTexture("NormalTex", CamDeferrdTarget->GetTexture(2));
		CalLightUnit.ShaderResHelper.SetTexture("MatTex", CamDeferrdTarget->GetTexture(3));
		CalLightUnit.ShaderResHelper.SetTexture("GleamTex", CamDeferrdTarget->GetTexture(4));
		//CalLightUnit.ShaderResHelper.SetTexture("SSSTex", CamDeferrdTarget->GetTexture(5));

		DefferdMergeUnit.SetMesh("FullRect");
		DefferdMergeUnit.SetMaterial("DeferredMerge");
		DefferdMergeUnit.ShaderResHelper.SetTexture("DifColor", CamDeferrdTarget->GetTexture(0));
		DefferdMergeUnit.ShaderResHelper.SetTexture("DifLight", DeferredLightTarget->GetTexture(0));
		DefferdMergeUnit.ShaderResHelper.SetTexture("SpcLight", DeferredLightTarget->GetTexture(1));
		DefferdMergeUnit.ShaderResHelper.SetTexture("AmbLight", DeferredLightTarget->GetTexture(2));
		//DefferdMergeUnit.ShaderResHelper.SetTexture("BackLight", DeferredLightTarget->GetTexture(3));
	}

	CamAlphaTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamAlphaTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamAlphaTarget->SetDepthTexture(CamTarget->GetDepthTexture());

	IsLoad = true;
}

void GameEngineCamera::RenderTargetTextureRelease()
{
	if (false == IsLoad)
	{
		return;
	}

	CalLightUnit.ShaderResHelper.AllResourcesRelease();
	DefferdMergeUnit.ShaderResHelper.AllResourcesRelease();

	CamTarget->ReleaseTextures();
	CamPosTarget->ReleaseTextures();
	CamForwardTarget->ReleaseTextures();

	if (nullptr != CamDeferrdTarget)
	{
		CamDeferrdTarget->ReleaseTextures();
		DeferredLightTarget->ReleaseTextures();
	}

	CamAlphaTarget->ReleaseTextures();
	IsLoad = false;
}

void GameEngineCamera::BakeShadow(std::shared_ptr<GameEngineLight> _BakeLight, int _BakeIndex /*= 0*/)
{	
	std::shared_ptr<GameEngineRenderTarget> BakeTarget = _BakeLight->GetBakeTarget(_BakeIndex);

	if (nullptr == BakeTarget)
	{
		MsgAssert("존재하지 않는 Shadow Bake Target을 Bake 하려 했습니다.")
	}

	BakeTarget->Clear();

	if (false == _BakeLight->IsShadow())
	{
		return;
	}

	std::shared_ptr<GameEngineMaterial> Pipe = nullptr;

	switch (_BakeLight->GetLightData().LightType)
	{
	case 1:
	case 2:
	{
		Pipe = GameEngineMaterial::Find("PShadow");
	}
	break;
	default:
	{
		Pipe = GameEngineMaterial::Find("OShadow");
	}
	break;
	}

	TransformData CurData = GetTransform()->GetTransDataRef();
	GetTransform()->SetWorldPosition(_BakeLight->GetTransform()->GetWorldPosition());

	_BakeLight->LightUpdate(this, 0.0f);

	for (size_t i = 0; i < _BakeLight->ViewDatas.size(); i++)
	{
		_BakeLight->LightViewSetting(i);
		BakeTarget->Setting(i);

		std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator LoopIter = StaticUnits.begin();
		std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndIter = StaticUnits.end();

		for (; LoopIter != EndIter; ++LoopIter)
		{
			std::shared_ptr<GameEngineRenderUnit> Unit = (*LoopIter);

			if (false == Unit->IsUpdate())
			{
				continue;
			}

			if (nullptr == Unit->GetRenderer())
			{
				continue;
			}

			if (false == Unit->GetRenderer()->IsUpdate())
			{
				continue;
			}

			if (false == Unit->IsShadow)
			{
				continue;
			}

			Unit->GetRenderer()->GetTransform()->SetCameraMatrix(_BakeLight->ViewDatas[i].LightViewMatrix, _BakeLight->ViewDatas[i].LightProjectionMatrix);
			TransformData Data = Unit->GetRenderer()->GetTransform()->GetTransDataRef();
			Unit->ShadowSetting();
			Pipe->VertexShader();
			Pipe->Rasterizer();
			Pipe->PixelShader();
			Pipe->OutputMerger();
			Unit->Draw();
		}
	}

	GetTransform()->SetTransformData(CurData);
}

void GameEngineCamera::PushStaticUnit(std::shared_ptr<GameEngineRenderUnit> _Unit)
{
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndIter = StaticUnits.end();
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator FindIter = std::find(StaticUnits.begin(), EndIter, _Unit);

	if (FindIter == EndIter)
	{
		StaticUnits.push_back(_Unit);
	}
}

void GameEngineCamera::PopStaticUnit(std::shared_ptr<GameEngineRenderUnit> _Unit)
{
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndIter = StaticUnits.end();
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator FindIter = std::find(StaticUnits.begin(), EndIter, _Unit);

	if (FindIter != EndIter)
	{
		StaticUnits.erase(FindIter);
	}
}

void GameEngineCamera::PushDynamicUnit(std::shared_ptr<GameEngineRenderUnit> _Unit)
{
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndIter = DynamicUnits.end();
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator FindIter = std::find(DynamicUnits.begin(), EndIter, _Unit);

	if (FindIter == EndIter)
	{
		DynamicUnits.push_back(_Unit);
	}
}

void GameEngineCamera::PopDynamicUnit(std::shared_ptr<GameEngineRenderUnit> _Unit)
{
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndIter = DynamicUnits.end();
	std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator FindIter = std::find(DynamicUnits.begin(), EndIter, _Unit);

	if (FindIter != EndIter)
	{
		DynamicUnits.erase(FindIter);
	}
}


void GameEngineCamera::FreeCameraSwitch()
{
	if (GetLevel()->GetMainCamera().get() != this)
	{
		return;
	}

	FreeCamera = !FreeCamera;

	if (true == FreeCamera)
	{
		OldData = GetTransform()->GetTransDataRef();
	}
	else
	{
		GetTransform()->SetTransformData(OldData);
	}
}

void GameEngineCamera::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("ProjectionModeChange"))
	{
		switch (ProjectionType)
		{
		case CameraType::None:
			break;
		case CameraType::Perspective:
			ProjectionType = CameraType::Orthogonal;
			break;
		case CameraType::Orthogonal:
			ProjectionType = CameraType::Perspective;
			break;
		default:
			break;
		}
	}

	if (true == FreeCamera)
	{
		float RotSpeed = 180.0f;
		float Speed = 200.0f;

		if (true == GameEngineInput::IsPress("SpeedBoost"))
		{
			Speed = 1000.0f;
		}

		if (true == GameEngineInput::IsPress("CamMoveLeft"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveRight"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveUp"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveDown"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveForward"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * Speed * _DeltaTime);
		}
		if (true == GameEngineInput::IsPress("CamMoveBack"))
		{
			GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * Speed * _DeltaTime);
		}

		if (true == GameEngineInput::IsPress("CamRot"))
		{
			float4 Dir = GameEngineInput::GetMouseDirectionNormal();

			float4 RotMouseDir;
			RotMouseDir.x = Dir.y;
			RotMouseDir.y = Dir.x;

			GetTransform()->AddWorldRotation(RotMouseDir);
		}
	}
}

void GameEngineCamera::ViewPortSetting()
{
	GameEngineDevice::GetContext()->RSSetViewports(1, &ViewPortData);
}

void GameEngineCamera::Setting()
{

}

void GameEngineCamera::Render(float _DeltaTime)
{
	CamForwardTarget->Clear();

	if (nullptr != CamDeferrdTarget)
	{
		CamDeferrdTarget->Clear();
		DeferredLightTarget->Clear();
	}

	CamAlphaTarget->Clear();

	{
		std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupStartIter = Renderers.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupEndIter = Renderers.end();

		for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineRenderer>>& RenderGroup = RenderGroupStartIter->second;

			int Order = RenderGroupStartIter->first;
			std::map<int, SortType>::iterator SortIter = SortValues.find(Order);

			if (SortIter != SortValues.end() && SortIter->second != SortType::None)
			{
				if (SortIter->second == SortType::ZSort)
				{
					for (std::shared_ptr<GameEngineRenderer>& Render : RenderGroup)
					{
						Render->CalSortZ(this);
					}

					RenderGroup.sort([](std::shared_ptr<GameEngineRenderer>& _Left, std::shared_ptr<GameEngineRenderer>& _Right)
						{
							return _Left->CalZ > _Right->CalZ;
						});
				}
			}

			std::list<std::shared_ptr<GameEngineRenderer>>::iterator StartRenderer = RenderGroup.begin();
			std::list<std::shared_ptr<GameEngineRenderer>>::iterator EndRenderer = RenderGroup.end();

			float ScaleTime = _DeltaTime * GameEngineTime::GlobalTime.GetRenderOrderTimeScale(RenderGroupStartIter->first);

			for (; StartRenderer != EndRenderer; ++StartRenderer)
			{
				std::shared_ptr<GameEngineRenderer>& Render = *StartRenderer;

				if (false == Render->IsUpdate())
				{
					continue;
				}

				if (true == Render->IsCameraCulling && false == IsView(Render->GetTransform()->GetTransDataRef()))
				{
					continue;
				}

				Render->RenderTransformUpdate(this);
				Render->RenderBaseValueUpdate(ScaleTime);
			}
		}
	}

	{
		GetLevel()->LightDataObject.LightCount = static_cast<int>(GetLevel()->AllLight.size());

		for (std::pair<const RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& Path : Units)
		{
			switch (Path.first)
			{
			case RenderPath::Forward:
			{
				CamForwardTarget->Setting();
			}
				break;
			case RenderPath::Deferred:
			{
				if (nullptr == CamDeferrdTarget)
				{
					continue;
				}

				CamDeferrdTarget->Setting();
			}
				break;
			case RenderPath::Alpha:
			{
				CamAlphaTarget->Setting();
			}
				break;
			default:
				continue;
			}

			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& UnitPath = Path.second;

			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupStartIter = UnitPath.begin();
			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupEndIter = UnitPath.end();

			for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
			{
				std::list<std::shared_ptr<GameEngineRenderUnit>>& RenderGroup = RenderGroupStartIter->second;

				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator StartRenderer = RenderGroup.begin();
				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndRenderer = RenderGroup.end();

				float ScaleTime = _DeltaTime * GameEngineTime::GlobalTime.GetRenderOrderTimeScale(RenderGroupStartIter->first);

				for (; StartRenderer != EndRenderer; ++StartRenderer)
				{
					std::shared_ptr<GameEngineRenderUnit>& Render = *StartRenderer;

					if (false == Render->IsUpdate())
					{
						continue;
					}

					if (false == Render->GetRenderer()->IsUpdate())
					{
						continue;
					}

					Render->Render(_DeltaTime);
				}
			}
		}

		GameEngineRenderUnit AniUnit;

		// 여기에서 이미 그림자를 그려야하는 애들은 다 그려져 있어야 합니다.
		for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
		{
			if (false == Light->IsShadow())
			{
				continue;
			}

			if (false == Light->IsDynamicLight)
			{
				continue;
			}

			std::shared_ptr<GameEngineMaterial> Pipe = nullptr;

			switch (Light->GetLightData().LightType)
			{
			case 1:
			case 2:
			{
				Pipe = GameEngineMaterial::Find("PShadow");
			}
			break;
			default:
			{
				Pipe = GameEngineMaterial::Find("OShadow");
			}
			break;
			}

			for (size_t i = 0; i < Light->ViewDatas.size(); i++)
			{
				Light->LightViewSetting(i);
				Light->GetShadowTarget()->Setting(i);

				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator LoopIter = DynamicUnits.begin();
				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndIter = DynamicUnits.end();

				for (; LoopIter != EndIter; ++LoopIter)
				{
					std::shared_ptr<GameEngineRenderUnit> Unit = (*LoopIter);

					if (false == Unit->IsUpdate())
					{
						continue;
					}

					if (nullptr == Unit->GetRenderer())
					{
						continue;
					}

					if (false == Unit->GetRenderer()->IsUpdate())
					{
						continue;
					}

					if (false == Unit->IsShadow)
					{
						continue;
					}

					Unit->GetRenderer()->GetTransform()->SetCameraMatrix(Light->ViewDatas[i].LightViewMatrix, Light->ViewDatas[i].LightProjectionMatrix);
					TransformData Data = Unit->GetRenderer()->GetTransform()->GetTransDataRef();
					Unit->ShadowSetting();
					Pipe->VertexShader();
					Pipe->Rasterizer();
					Pipe->PixelShader();
					Pipe->OutputMerger();
					Unit->Draw();
				}
			}
		}

		GameEngineRenderTarget::Reset();

		if (nullptr != CamDeferrdTarget)
		{
			DeferredLightTarget->Setting();
			GetLevel()->LightDataObject.LightCount = 0;

			for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
			{
				if (false == Light->IsShadow())
				{
					CalLightUnit.ShaderResHelper.SetTexture("ShadowTex", GameEngineTexture::Find("EngineNullDepth.png"));
					CalLightUnit.ShaderResHelper.SetTexture("PointShadowTex", GameEngineTexture::Find("BaseShadowCubemap"));
				}
				else
				{
					if (Light->GetLightData().LightType == static_cast<int>(LightType::Point))
					{
						CalLightUnit.ShaderResHelper.SetTexture("PointShadowTex", Light->GetShadowTarget()->GetTexture(0));
						CalLightUnit.ShaderResHelper.SetTexture("ShadowTex", GameEngineTexture::Find("EngineNullDepth.png"));
					}
					else
					{
						CalLightUnit.ShaderResHelper.SetTexture("PointShadowTex", GameEngineTexture::Find("BaseShadowCubemap"));
						CalLightUnit.ShaderResHelper.SetTexture("ShadowTex", Light->GetShadowTarget()->GetTexture(0));
					}
				}

				CalLightUnit.Render(_DeltaTime);
				++GetLevel()->LightDataObject.LightCount;
			}

			DeferredLightTarget->Effect(_DeltaTime);
			CalLightUnit.ShaderResHelper.AllResourcesReset();
		}

		CamTarget->Clear();
		CamTarget->Merge(CamForwardTarget);

		if (nullptr != CamDeferrdTarget)
		{
			CamTarget->Setting();

			DefferdMergeUnit.Render(_DeltaTime);
			DefferdMergeUnit.ShaderResHelper.AllResourcesReset();
		}

		CamTarget->Merge(CamAlphaTarget);

		CamPosTarget->Clear();
		CamPosTarget->Merge(CamForwardTarget, 1);

		if (nullptr != CamDeferrdTarget)
		{		
			CamPosTarget->Merge(CamDeferrdTarget, 1);
		}

		CamTarget->Effect(_DeltaTime);
	}
}

void GameEngineCamera::CameraTransformUpdate()
{
	float4 EyeDir = GetTransform()->GetLocalForwardVector();
	float4 EyeUp = GetTransform()->GetLocalUpVector();
	float4 EyePos = GetTransform()->GetLocalPosition();

	View.LookToLH(EyePos, EyeDir, EyeUp);

	switch (ProjectionType)
	{
	case CameraType::None:
	{
		MsgAssert("카메라 투영이 설정되지 않았습니다.");
		break;
	}
	case CameraType::Perspective:
		Projection.PerspectiveFovLH(FOV, Width / Height, Near, Far);
		break;
	case CameraType::Orthogonal:
		Projection.OrthographicLH(Width * ZoomRatio, Height * ZoomRatio, Near, Far);
		break;
	default:
		break;
	}

	ViewPort.ViewPort(Width, Height, 0.0f, 0.0f);

	float4 WorldPos = GetTransform()->GetWorldPosition();
	float4 Dir = GetTransform()->GetLocalForwardVector();
	Box.Center = (WorldPos + (Dir * Far * 0.5f)).DirectFloat3;
	Box.Extents.z = Far * 0.6f;
	Box.Extents.x = Width * 0.6f;
	Box.Extents.y = Height * 0.6f;
	Box.Orientation = GetTransform()->GetWorldQuaternion().DirectFloat4;
}

void GameEngineCamera::PushRenderer(std::shared_ptr<GameEngineRenderer> _Render)
{
	if (nullptr == _Render)
	{
		MsgAssert("랜더러가 nullptr 입니다");
		return;
	}

	_Render->RenderCamera = this;

	Renderers[_Render->GetOrder()].push_back(_Render);
}

void GameEngineCamera::PushRenderUnit(std::shared_ptr<GameEngineRenderUnit> _Unit, RenderPath _Path /*= RenderPath::None*/)
{
	if (nullptr == _Unit->GetRenderer())
	{
		MsgAssert("부모가 없는 랜더유니트입니다");
		return;
	}

	int Order = _Unit->GetRenderer()->GetOrder();

	RenderPath Path = _Unit->Material->GetPixelShader()->GetRenderPath();

	if (_Path != RenderPath::None)
	{
		Path = _Path;
	}

	Units[Path][Order].push_back(_Unit);

	if (true == _Unit->IsStatic)
	{
		PushStaticUnit(_Unit);
	}
	else
	{
		PushDynamicUnit(_Unit);
	}
}

void GameEngineCamera::PopRenderUnit(std::shared_ptr<GameEngineRenderUnit> _Unit, RenderPath _Path)
{
	if (nullptr == _Unit->GetRenderer())
	{
		MsgAssert("부모가 없는 랜더유니트입니다");
		return;
	}

	int Order = _Unit->GetRenderer()->GetOrder();

	RenderPath Path = _Unit->Material->GetPixelShader()->GetRenderPath();

	if (_Path != RenderPath::None)
	{
		Path = _Path;
	}

	if (true == _Unit->IsStatic)
	{
		PopStaticUnit(_Unit);
	}
	else
	{
		PopDynamicUnit(_Unit);
	}

	Units[Path][Order].remove(_Unit);
}

bool GameEngineCamera::IsView(const TransformData& _TransData)
{
	if (true == IsFreeCamera())
	{
		return true;
	}

	switch (ProjectionType)
	{
	case CameraType::None:
	{
		MsgAssert("카메라 투영이 설정되지 않았습니다.");
		break;
	}
	case CameraType::Perspective:
		break;
	case CameraType::Orthogonal:
	{

		DirectX::BoundingSphere Sphere;
		Sphere.Center = _TransData.WorldPosition.DirectFloat3;
		Sphere.Radius = _TransData.WorldScale.MaxFloat() * 0.5f;

		bool IsCal = Box.Intersects(Sphere);

		return IsCal;
	}
	default:
		break;
	}

	return false;
}

void GameEngineCamera::Release()
{
	{
		std::list<std::shared_ptr<class GameEngineRenderUnit>>::iterator StaticLoopIter = StaticUnits.begin();
		std::list<std::shared_ptr<class GameEngineRenderUnit>>::iterator StaticEndIter = StaticUnits.end();

		for (; StaticLoopIter != StaticEndIter; )
		{
			if (true == (*StaticLoopIter)->GetRenderer()->IsDeath())
			{
				StaticLoopIter = StaticUnits.erase(StaticLoopIter);
			}
			else
			{
				++StaticLoopIter;
			}
		}

		std::list<std::shared_ptr<class GameEngineRenderUnit>>::iterator DynamicLoopIter = DynamicUnits.begin();
		std::list<std::shared_ptr<class GameEngineRenderUnit>>::iterator DynamicEndIter = DynamicUnits.end();

		for (; DynamicLoopIter != DynamicEndIter; )
		{
			if (true == (*DynamicLoopIter)->GetRenderer()->IsDeath())
			{
				DynamicLoopIter = DynamicUnits.erase(DynamicLoopIter);
			}
			else
			{
				++DynamicLoopIter;
			}
		}

		for (std::pair<const RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& Path : Units)
		{
			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& UnitPath = Path.second;

			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupStartIter = UnitPath.begin();
			std::map<int, std::list<std::shared_ptr<GameEngineRenderUnit>>>::iterator RenderGroupEndIter = UnitPath.end();


			for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
			{
				std::list<std::shared_ptr<GameEngineRenderUnit>>& RenderGroup = RenderGroupStartIter->second;

				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator StartRenderUnit = RenderGroup.begin();
				std::list<std::shared_ptr<GameEngineRenderUnit>>::iterator EndRenderUnit = RenderGroup.end();

				float ScaleTime = GameEngineTime::GlobalTime.GetRenderOrderTimeScale(RenderGroupStartIter->first);

				for (; StartRenderUnit != EndRenderUnit; /*++StartRenderer*/)
				{
					std::shared_ptr<GameEngineRenderUnit>& Render = *StartRenderUnit;

					if (false == Render->GetRenderer()->IsDeath())
					{
						++StartRenderUnit;
						continue;
					}

					StartRenderUnit = RenderGroup.erase(StartRenderUnit);
				}
			}
		}

		std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupStartIter = Renderers.begin();
		std::map<int, std::list<std::shared_ptr<GameEngineRenderer>>>::iterator RenderGroupEndIter = Renderers.end();

		for (; RenderGroupStartIter != RenderGroupEndIter; ++RenderGroupStartIter)
		{
			std::list<std::shared_ptr<GameEngineRenderer>>& RenderGroup = RenderGroupStartIter->second;

			std::list<std::shared_ptr<GameEngineRenderer>>::iterator StartRenderer = RenderGroup.begin();
			std::list<std::shared_ptr<GameEngineRenderer>>::iterator EndRenderer = RenderGroup.end();

			for (; StartRenderer != EndRenderer;)
			{
				std::shared_ptr<GameEngineRenderer>& Render = *StartRenderer;

				if (false == Render->IsDeath())
				{
					++StartRenderer;
					continue;
				}

				StartRenderer = RenderGroup.erase(StartRenderer);
			}
		}
	}
}