#include "PrecompileHeader.h"
#include "OldFilm.h"

OldFilm::OldFilm() 
{
}

OldFilm::~OldFilm() 
{
}

//
void OldFilm::Start(GameEngineRenderTarget* _Target)
{
	// 리소스 로드
	if (nullptr == GameEngineSprite::Find("OldFilm"))
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		GameEngineSprite::LoadFolder(NewDir.GetPlusFileName("OldFilm").GetFullPath());
	}

	OldUnit = std::make_shared<GameEngineRenderUnit>();
	OldUnit->SetMesh("FullRect");
	OldUnit->SetPipeLine("OldFilm");

	OldData.x = 10.0f;

	OldUnit->ShaderResHelper.SetConstantBufferLink("OldFilmData", OldData);


	ResultTarget = GameEngineRenderTarget::Create(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, GameEngineWindow::GetScreenSize(), float4::Null);
}

void OldFilm::Effect(GameEngineRenderTarget* _Target, float _DeltaTime)
{
	std::shared_ptr<GameEngineSprite> Sprite = GameEngineSprite::Find("OldFilm");
	SpriteInfo Info = Sprite->GetSpriteInfo(Index);

	if (Sprite->GetSpriteCount() <= ++Index)
	{
		Index = 0;
	}

	OldUnit->ShaderResHelper.SetTexture("DiffuseTex", Info.Texture);

	ResultTarget->Clear();
	ResultTarget->Setting();
	OldUnit->Render(_DeltaTime);
	OldUnit->ShaderResHelper.AllResourcesReset();

	// _Target->Clear();
	_Target->Merge(ResultTarget);

}