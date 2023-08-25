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

std::shared_ptr<FieldMap> FieldMap::CreateFieldMap(GameEngineLevel* _Level, const std::string_view& _FBXName, const std::vector<FieldMapColData>& _ColVector, const float4& _MapPosition)
{
	std::shared_ptr<FieldMap> Result;
	Result = _Level->CreateActor<FieldMap>();
	Result->GetTransform()->SetLocalPosition(_MapPosition); //맵의 중심 위치를 정합니다
	Result->FieldMapRenderer = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->FieldMapRenderer->SetFBXMesh(_FBXName.data(), "NoneAlphaMesh"); //메쉬를 깝니다

	if (_ColVector.begin() != _ColVector.end())
	{
		Result->MapCols.resize(_ColVector.size());
		for (size_t i = 0; i < Result->MapCols.size(); i++) //충돌체를 깝니다
		{
			Result->MapCols[i] = Result->CreateComponent<GameEngineCollision>(_ColVector[i].ColOrder);
			Result->MapCols[i]->SetColType(_ColVector[i].Type);
			Result->MapCols[i]->GetTransform()->SetLocalPosition(_ColVector[i].Pos);
			Result->MapCols[i]->GetTransform()->SetLocalScale(_ColVector[i].Scale);
			Result->MapCols[i]->GetTransform()->SetLocalRotation(_ColVector[i].Rot);
		}
	}

	return Result;
}

void FieldMap::Start()
{
}

void FieldMap::Update(float _DeltaTime)
{
}
