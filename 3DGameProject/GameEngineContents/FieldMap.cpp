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

std::shared_ptr<FieldMap> FieldMap::CreateFieldMap(GameEngineLevel* _Level, const std::string_view _FBXName, const std::vector<FieldMapColData>& _ColVector, const float4& _MapPosition)
{
	std::shared_ptr<FieldMap> Result;
	Result = _Level->CreateActor<FieldMap>();
	Result->GetTransform()->SetLocalPosition(_MapPosition);
	Result->FieldMapRenderer = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->FieldMapRenderer->SetFBXMesh(_FBXName.data(), "NoneAlphaMesh");
	Result->FieldMapRenderer->GetMaterial()->SetFILL_MODE(D3D11_FILL_WIREFRAME);

	Result->MapCols.resize(_ColVector.size());
	for (size_t i = 0; i < Result->MapCols.size(); i++)
	{
		Result->MapCols[i] = Result->CreateComponent<GameEngineCollision>(_ColVector[i].ColOrder);
		Result->MapCols[i]->SetColType(_ColVector[i].Type);
		Result->MapCols[i]->GetTransform()->SetLocalPosition(_ColVector[i].Pos);
		Result->MapCols[i]->GetTransform()->SetLocalScale(_ColVector[i].Scale);
		Result->MapCols[i]->GetTransform()->SetLocalRotation(_ColVector[i].Rot);
	}

	return Result;
}

void FieldMap::Start()
{
}

void FieldMap::Update(float _DeltaTime)
{
}
