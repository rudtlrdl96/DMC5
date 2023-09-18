#include "PrecompileHeader.h"
#include "GameEngineCoreWindow.h"
#include <GameEngineBase/GameEngineString.h>

std::map<int, NameRenderTarget> GameEngineCoreWindow::DebugRenderTarget;


void GameEngineImageShotWindow::RenderTextureSetting(ImTextureID _RenderTexture, ImVec2 _Size)
{
	RenderTexture = _RenderTexture;
	Size = _Size;
}

void GameEngineImageShotWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	if (true == ImGui::ImageButton(RenderTexture, { Size.x, Size.y }))
	{
		Off();
	}

}


GameEngineCoreWindow::GameEngineCoreWindow()
{
}

GameEngineCoreWindow::~GameEngineCoreWindow()
{
}


void GameEngineCoreWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	for (std::pair<int, NameRenderTarget> RenderTargetPair : DebugRenderTarget)
	{
		std::string Name = RenderTargetPair.second.Name;
		// 트리노드라는 Imgui 기능을 사용할것이다.
		if (true == ImGui::TreeNodeEx(Name.c_str(), 0))
		{
			std::shared_ptr<GameEngineRenderTarget> RenderTarget = RenderTargetPair.second.RenderTarget;

			int Index = 0;

			for (ID3D11ShaderResourceView* _View : RenderTarget->SRVs)
			{
				float4 Scale = RenderTarget->Textures[Index]->GetScale();
				float4 SmallScale = Scale * 0.2f;

				++Index;

				if (true == ImGui::ImageButton(static_cast<ImTextureID>(_View), { SmallScale.x, SmallScale.y }))
				{
					std::shared_ptr<GameEngineGUIWindow> NewWindow = nullptr;
					if (nullptr == GameEngineGUI::FindGUIWindow("ImageShot"))
					{
						NewWindow = GameEngineGUI::GUIWindowCreate<GameEngineImageShotWindow>("ImageShot");
					}
					else
					{
						NewWindow = GameEngineGUI::FindGUIWindow("ImageShot");
					}

					NewWindow->On();
					std::shared_ptr<GameEngineImageShotWindow> Window = std::dynamic_pointer_cast<GameEngineImageShotWindow>(NewWindow);
					Window->RenderTextureSetting(static_cast<ImTextureID>(_View), { Scale.x ,Scale.y });
				}
			}

			ImGui::TreePop();
		}
	}


}

void GameEngineCoreWindow::AddDebugRenderTarget(int _Order, const std::string_view& _Name, std::shared_ptr<GameEngineRenderTarget> _Target)
{
	// _Target->SetName(_Name);
	DebugRenderTarget[_Order] = { _Name.data(), _Target };
}

void GameEngineCoreWindow::Clear()
{
	DebugRenderTarget.clear();
}