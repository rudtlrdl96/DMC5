#include "PrecompileHeader.h"
#include "GameEngineCamera.h"
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEnginePlatform/GameEngineWindow.h>
#include "GameEngineDevice.h"
#include "GameEngineRenderer.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineTexture.h"

GameEngineCamera::GameEngineCamera()
{
	SetName("GameEngineCamera");
}

GameEngineCamera::~GameEngineCamera()
{
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
		GameEngineInput::AddIgnoreKeys("FreeCameraSwitch");
	}

	ViewPortData.TopLeftX = 0;
	ViewPortData.TopLeftY = 0;
	ViewPortData.Width = GameEngineWindow::GetScreenSize().x;
	ViewPortData.Height = GameEngineWindow::GetScreenSize().y;
	ViewPortData.MinDepth = 0.0f;
	ViewPortData.MaxDepth = 1.0f;

	Width = ViewPortData.Width;
	Height = ViewPortData.Height;

	CamTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	CamTarget->CreateDepthTexture();	
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
	else {
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

void GameEngineCamera::Setting()
{
	GameEngineDevice::GetContext()->RSSetViewports(1, &ViewPortData);
	CamTarget->Clear();
	CamTarget->Setting();
}

void GameEngineCamera::Render(float _DeltaTime)
{
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
		for (std::pair<const int, std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>>& UnitGroup : Units)
		{
			std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& UnitPath = UnitGroup.second;

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

	ViewPort.ViewPort(GameEngineWindow::GetScreenSize().x, GameEngineWindow::GetScreenSize().y, 0.0f, 0.0f);

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

void GameEngineCamera::PushRenderUnit(std::shared_ptr<GameEngineRenderUnit> _Unit)
{
	if (nullptr == _Unit->GetRenderer())
	{
		MsgAssert("부모가 없는 랜더유니트입니다");
		return;
	}

	int Order = _Unit->GetRenderer()->GetOrder();

	Units[0][Order].push_back(_Unit);
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
		std::map<int, std::list<std::shared_ptr<class GameEngineRenderUnit>>>& UnitPath = Units[0];

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