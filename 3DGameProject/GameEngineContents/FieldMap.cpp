#include "PrecompileHeader.h"
#include "FieldMap.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineMaterial.h>
#include <GameEngineCore/GameEngineCollision.h>

FieldMap::FieldMap()
{

}

FieldMap::~FieldMap()
{
	
}

std::shared_ptr<FieldMap> FieldMap::CreateFieldMap(GameEngineLevel* _Level, const std::vector<std::string>& _FBXNames, const std::vector<ObjTransformData>& _CullingCols/*, const std::vector<int>& _NodeIndex*/)
{
	std::shared_ptr<FieldMap> Result;
	Result = _Level->CreateActor<FieldMap>();
	Result->SetName("FieldMap");
	std::vector<std::shared_ptr<GameEngineFBXRenderer>>& MapRenderersRef = Result->FieldMapRenderer;
	MapRenderersRef.resize(_FBXNames.size());
	for (size_t i = 0; i < MapRenderersRef.size(); i++)
	{
		MapRenderersRef[i] = Result->CreateComponent<GameEngineFBXRenderer>();
		if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
		{
			MapRenderersRef[i]->SetFBXMesh(_FBXNames[i].data(), "FBX_Low");
		}
		else
		{
			MapRenderersRef[i]->SetFBXMesh(_FBXNames[i].data(), "FBX");
		}
	}

	std::vector<std::shared_ptr<GameEngineCollision>>& MapCullingColRef = Result->FieldMapCullingCol;
	MapCullingColRef.resize(_CullingCols.size());
	for (size_t i = 0; i < MapCullingColRef.size(); i++)
	{
		MapCullingColRef[i] = Result->CreateComponent<GameEngineCollision>();
		MapCullingColRef[i]->SetColType(ColType::OBBBOX3D);
		MapCullingColRef[i]->GetTransform()->SetLocalPosition(_CullingCols[i].Pos);
		MapCullingColRef[i]->GetTransform()->SetLocalScale(_CullingCols[i].Scale);
		MapCullingColRef[i]->GetTransform()->SetLocalRotation(_CullingCols[i].Rot);
	}

	//std::vector<int>& NodeIndexRef = Result->NodeIndex;
	//NodeIndexRef.resize(_NodeIndex.size());
	//for (size_t i = 0; i < NodeIndexRef.size(); i++)
	//{
	//	NodeIndexRef[i] = _NodeIndex[i];
	//}

	return Result;
}

void FieldMap::EraseFieldMap()
{
	ClearFieldMapRenderer();
	ClearFieldMapCullingCol();
}

#include <GameEngineCore/imgui.h>

void FieldMap::DrawEditor()
{
	ImGui::Spacing();
	ImGui::Spacing();

	float4 Color = FieldMapRenderer[0]->GetBaseColor();

	float InputColor[4] = { Color.x, Color.y, Color.z, Color.w };
	ImGui::DragFloat4("Color", InputColor, 0.01f, 0.0f, 1.0f);

	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i]->SetBaseColor(float4(InputColor[0], InputColor[1], InputColor[2], InputColor[3]));
	}
}

void FieldMap::ClearFieldMapRenderer()
{
	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i]->Death();
		FieldMapRenderer[i] = nullptr;
	}
	FieldMapRenderer.clear();
}

void FieldMap::ClearFieldMapCullingCol()
{
	for (size_t i = 0; i < FieldMapCullingCol.size(); i++)
	{
		FieldMapCullingCol[i]->Death();
		FieldMapCullingCol[i] = nullptr;
	}
	FieldMapCullingCol.clear();
}

