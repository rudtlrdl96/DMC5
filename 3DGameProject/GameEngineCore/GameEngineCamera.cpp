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
	CamForwardTarget->Clear();
	CamDeferrdTarget->Clear();
	CamAlphaTarget->Clear();
	AllRenderTarget->Clear();
	AlphaRenderTarget->Clear();

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

	AllRenderTarget->Setting();
	Render(0.0f);

	Width = CurWidth;
	Height = CurHeight;
	
	GetTransform()->SetTransformData(CurTransData);
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

	// ť��
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

	AllRenderTarget = GameEngineRenderTarget::Create();
	AlphaRenderTarget = GameEngineRenderTarget::Create();
	CamTarget = GameEngineRenderTarget::Create();
	DeferredLightTarget = GameEngineRenderTarget::Create();
	CamForwardTarget = GameEngineRenderTarget::Create();
	CamDeferrdTarget = GameEngineRenderTarget::Create();
	CamAlphaTarget = GameEngineRenderTarget::Create();

	//CubeRenderTarget = GameEngineRenderTarget::Create();
}

void GameEngineCamera::RenderTargetTextureLoad()
{
	if (true == IsLoad)
	{
		return;
	}

	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	AllRenderTarget->CreateDepthTexture();

	AlphaRenderTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	
	CamTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // ������, ��ǻ�� ����Ʈ
	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // ����ŧ�� ����Ʈ
	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // �ں��Ʈ ����Ʈ�� ��´�.
	DeferredLightTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL); // ���

	CamForwardTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	CamDeferrdTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);

	CamAlphaTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamAlphaTarget->CreateDepthTexture();

	//CubeRenderTarget->CreateCubeTexture();
	//CubeRenderTarget->CreateCubeDepth();

	CalLightUnit.SetMesh("FullRect");
	CalLightUnit.SetMaterial("DeferredCalLight");

	LightDatas& Data = GetLevel()->LightDataObject;
	CalLightUnit.ShaderResHelper.SetConstantBufferLink("LightDatas", Data);
	CalLightUnit.ShaderResHelper.SetTexture("PositionTex", AllRenderTarget->GetTexture(2));
	CalLightUnit.ShaderResHelper.SetTexture("NormalTex", AllRenderTarget->GetTexture(3));
	CalLightUnit.ShaderResHelper.SetTexture("MatTex", AllRenderTarget->GetTexture(4));
	CalLightUnit.ShaderResHelper.SetTexture("GleamTex", AllRenderTarget->GetTexture(5));

	DefferdMergeUnit.SetMesh("FullRect");
	DefferdMergeUnit.SetMaterial("DeferredMerge");
	DefferdMergeUnit.ShaderResHelper.SetTexture("DifColor", AllRenderTarget->GetTexture(1));
	DefferdMergeUnit.ShaderResHelper.SetTexture("DifLight", DeferredLightTarget->GetTexture(0));
	DefferdMergeUnit.ShaderResHelper.SetTexture("SpcLight", DeferredLightTarget->GetTexture(1));
	DefferdMergeUnit.ShaderResHelper.SetTexture("AmbLight", DeferredLightTarget->GetTexture(2));

	AlphaMergeUnit.SetMesh("FullRect");
	AlphaMergeUnit.SetMaterial("AlphaMerge");
	AlphaMergeUnit.ShaderResHelper.SetTexture("DiffuseTex", CamAlphaTarget->GetTexture(0));

	IsLoad = true;
}

void GameEngineCamera::RenderTargetTextureRelease()
{
	if (false == IsLoad)
	{
		return;
	}

	AlphaMergeUnit.ShaderResHelper.AllResourcesRelease();
	CalLightUnit.ShaderResHelper.AllResourcesRelease();
	DefferdMergeUnit.ShaderResHelper.AllResourcesRelease();

	AllRenderTarget->ReleaseTextures();
	CamTarget->ReleaseTextures();
	CamForwardTarget->ReleaseTextures();
	CamDeferrdTarget->ReleaseTextures();
	CamAlphaTarget->ReleaseTextures();
	DeferredLightTarget->ReleaseTextures();
	AlphaRenderTarget->ReleaseTextures();

	IsLoad = false;
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
	AllRenderTarget->Clear();
	AllRenderTarget->Setting();

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

	DeferredLightTarget->Clear();
	CamAlphaTarget->Clear();

	{
		for (std::pair<const RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& Path : Units)
		{
			if (Path.first == RenderPath::Alpha)
			{
				CamAlphaTarget->Setting();
			}
			else
			{
				AllRenderTarget->Setting();
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

		// ���⿡�� �̹� �׸��ڸ� �׷����ϴ� �ֵ��� �� �׷��� �־�� �մϴ�.
		for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
		{
			if (false == Light->IsShadow())
			{
				continue;
			}

			Light->GetShadowTarget()->Setting();

			for (std::pair<const RenderPath, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& Path : Units)
			{
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

						if (false == Render->IsShadow)
						{
							continue;
						}

						Render->GetRenderer()->GetTransform()->SetCameraMatrix(Light->GetLightData().LightViewMatrix, Light->GetLightData().LightProjectionMatrix);
						TransformData Data = Render->GetRenderer()->GetTransform()->GetTransDataRef();
						Render->Setting();
						std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Find("Shadow");
						Pipe->VertexShader();
						Pipe->Rasterizer();
						Pipe->PixelShader();
						Pipe->OutputMerger();
						Render->Draw();
					}
				}
			}
		}

		GameEngineRenderTarget::Reset();

		DeferredLightTarget->Setting();

		for (std::shared_ptr<GameEngineLight> Light : GetLevel()->AllLight)
		{
			if (false == Light->IsShadow())
			{
				continue;
			}

			CalLightUnit.ShaderResHelper.SetTexture("ShadowTex", Light->GetShadowTarget()->GetTexture(0));
			CalLightUnit.Render(_DeltaTime);
		}
				
		DeferredLightTarget->Effect(_DeltaTime);

		CamDeferrdTarget->Clear();
		CamDeferrdTarget->Setting();
		DefferdMergeUnit.Render(_DeltaTime);
		DefferdMergeUnit.ShaderResHelper.AllResourcesReset();

		CamForwardTarget->Clear();
		CamForwardTarget->Merge(AllRenderTarget);

		CamTarget->Clear();
		CamTarget->Merge(CamForwardTarget);
		CamTarget->Merge(CamDeferrdTarget);

		CamTarget->Setting();
		AlphaMergeUnit.Render(0.0f);
		AlphaMergeUnit.ShaderResHelper.AllResourcesReset();

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
		MsgAssert("ī�޶� ������ �������� �ʾҽ��ϴ�.");
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
		MsgAssert("�������� nullptr �Դϴ�");
		return;
	}

	_Render->RenderCamera = this;

	Renderers[_Render->GetOrder()].push_back(_Render);
}

void GameEngineCamera::PushRenderUnit(std::shared_ptr<GameEngineRenderUnit> _Unit, RenderPath _Path /*= RenderPath::None*/)
{
	if (nullptr == _Unit->GetRenderer())
	{
		MsgAssert("�θ� ���� ��������Ʈ�Դϴ�");
		return;
	}

	int Order = _Unit->GetRenderer()->GetOrder();

	RenderPath Path = _Unit->Material->GetPixelShader()->GetRenderPath();

	if (_Path != RenderPath::None)
	{
		Path = _Path;
	}

	Units[Path][Order].push_back(_Unit);
}

void GameEngineCamera::PopRenderUnit(std::shared_ptr<GameEngineRenderUnit> _Unit, RenderPath _Path)
{
	if (nullptr == _Unit->GetRenderer())
	{
		MsgAssert("�θ� ���� ��������Ʈ�Դϴ�");
		return;
	}

	int Order = _Unit->GetRenderer()->GetOrder();

	RenderPath Path = _Unit->Material->GetPixelShader()->GetRenderPath();

	if (_Path != RenderPath::None)
	{
		Path = _Path;
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
		MsgAssert("ī�޶� ������ �������� �ʾҽ��ϴ�.");
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