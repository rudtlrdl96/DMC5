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

//std::shared_ptr<FieldMap> FieldMap::CreateFieldMap(GameEngineLevel* _Level, const std::string_view& _FBXName, const std::vector<FieldMapColData>& _ColVector, const float4& _MapPosition, const float4& _MapScale, const float4& _MapRotation)
//{
//	std::shared_ptr<FieldMap> Result;
//	Result = _Level->CreateActor<FieldMap>();
//	Result->GetTransform()->SetLocalPosition(_MapPosition); //맵의 중심 위치를 정합니다
//	Result->GetTransform()->SetLocalScale(_MapScale);
//	Result->GetTransform()->SetLocalRotation(_MapRotation);
//	Result->FieldMapRenderer = Result->CreateComponent<GameEngineFBXRenderer>();
//	Result->FieldMapRenderer->SetFBXMesh(_FBXName.data(), "FBX"); //메쉬를 깝니다
//
//	if (_ColVector.begin() != _ColVector.end())
//	{
//		Result->MapCols.resize(_ColVector.size());
//		for (size_t i = 0; i < Result->MapCols.size(); i++) //충돌체를 깝니다
//		{
//			Result->MapCols[i] = Result->CreateComponent<GameEngineCollision>(_ColVector[i].ColOrder);
//			Result->MapCols[i]->SetColType(_ColVector[i].Type);
//			Result->MapCols[i]->GetTransform()->SetLocalPosition(_ColVector[i].Pos);
//			Result->MapCols[i]->GetTransform()->SetLocalScale(_ColVector[i].Scale);
//			Result->MapCols[i]->GetTransform()->SetLocalRotation(_ColVector[i].Rot);
//		}
//	}
//
//	return Result;
//}

std::shared_ptr<FieldMap> FieldMap::CreateFieldMap(GameEngineLevel* _Level, const std::string_view& _FBXName, const float4& _MapPosition, const float4& _MapScale, const float4& _MapRotation)
{
	std::shared_ptr<FieldMap> Result;
	Result = _Level->CreateActor<FieldMap>();
	Result->SetName("FieldMap");
	Result->GetTransform()->SetLocalPosition(_MapPosition); //맵의 중심 위치를 정합니다
	Result->GetTransform()->SetLocalScale(_MapScale);
	Result->GetTransform()->SetLocalRotation(_MapRotation);
	Result->FieldMapRenderer = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->FieldMapRenderer->SetName("Renderer");
	Result->FieldMapRenderer->SetFBXMesh(_FBXName.data(), "FBX"); //메쉬를 깝니다

	return Result;
}

void FieldMap::Start()
{
}

void FieldMap::Update(float _DeltaTime)
{
}

#include <GameEngineCore/imgui.h>

void FieldMap::DrawEditor()
{
	ImGui::Spacing();
	ImGui::Spacing();

	float4 Color = FieldMapRenderer->GetBaseColor();

	float InputColor[4] = { Color.x, Color.y, Color.z, Color.w };
	ImGui::DragFloat4("Color", InputColor, 0.01f, 0.0f, 1.0f);

	FieldMapRenderer->SetBaseColor(float4(InputColor[0], InputColor[1], InputColor[2], InputColor[3]));

}
