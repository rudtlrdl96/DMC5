#include "PrecompileHeader.h"
#include "EffectFBXRenderer.h"

EffectFBXRenderer::EffectFBXRenderer() 
{
}

EffectFBXRenderer::~EffectFBXRenderer() 
{
}

void EffectFBXRenderer::Start()
{
	GameEngineFBXRenderer::Start();
}


void EffectFBXRenderer::SetFBXMesh(const std::string& _Name, std::string _Material)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material);
	ClipSetting();
}

void EffectFBXRenderer::SetFBXMesh(const std::string& _Name, const std::vector<std::vector<std::string>>& _Materials)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Materials);
	ClipSetting();
}

void EffectFBXRenderer::SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, MeshIndex);
	ClipSetting();
}

void EffectFBXRenderer::SetFBXMesh(const std::string& _Name, std::vector<std::string> _Material, size_t MeshIndex)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, MeshIndex);
	ClipSetting();
}

void EffectFBXRenderer::ClipSetting()
{
	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("ClipData", Clip);
		}
	}
}

#include <GameEngineCore/imgui.h>

void EffectFBXRenderer::DrawEditor()
{
	ImGui::DragFloat("Clip Start X", &Clip.ClipStartX, 0.01f);
	ImGui::DragFloat("Clip End X", &Clip.ClipEndX, 0.01f);

	ImGui::DragFloat("Clip Start Y", &Clip.ClipStartY, 0.01f);
	ImGui::DragFloat("Clip End Y", &Clip.ClipEndY, 0.01f);
}