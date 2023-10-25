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
	Result->SetName("SkyBox");
	Result->Sky = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->Sky->SetName("Renderer");
	Result->Sky->SetFBXMesh(_FBXName.data(), "FBX");
	Result->Sky->LightOff();
	Result->Sky->ShadowOff();
	Result->Sky->SetBaseColor({ 2.0f,0.f,0.f });
	return Result;
}

void SkyBox::SetSkyBloom(float _Bloom)
{
	Sky->SetBaseColor({ _Bloom,0.f,0.f });
}
