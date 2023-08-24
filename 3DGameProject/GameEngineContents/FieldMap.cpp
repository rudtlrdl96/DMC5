#include "PrecompileHeader.h"
#include "FieldMap.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineMaterial.h>

FieldMap::FieldMap()
{

}

FieldMap::~FieldMap()
{

}

std::shared_ptr<FieldMap> FieldMap::CreateFieldMap(GameEngineLevel* _Level, std::string_view _FBXName)
{
	std::shared_ptr<FieldMap> Result;
	Result = _Level->CreateActor<FieldMap>();
	Result->FieldMapRenderer = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->FieldMapRenderer->SetFBXMesh(_FBXName.data(), "NoneAlphaMesh");
	Result->FieldMapRenderer->GetMaterial()->SetFILL_MODE(D3D11_FILL_WIREFRAME);
	return Result;
}

void FieldMap::Start()
{
}

void FieldMap::Update(float _DeltaTime)
{
}
