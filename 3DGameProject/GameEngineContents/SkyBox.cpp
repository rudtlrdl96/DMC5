#include "PrecompileHeader.h"
#include "SkyBox.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>


SkyBox::SkyBox()
{

}

SkyBox::~SkyBox()
{

}

std::shared_ptr<SkyBox> SkyBox::CreateSkyBox(GameEngineLevel* _Level, const std::string_view& _FBXName)
{
	std::shared_ptr<SkyBox> Result;
	Result = _Level->CreateActor<SkyBox>();
	Result->Sky = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->Sky->SetFBXMesh(_FBXName.data(), "NoneAlphaMesh");
	Result->Sky->LightOff();
	return Result;
}

void SkyBox::Start()
{
}

void SkyBox::Update(float _DeltaTime)
{
}
