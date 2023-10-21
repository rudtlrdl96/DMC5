 #pragma once
#include <GameEngineCore/GameEngineRenderTarget.h>
#include <GameEngineCore/GameEngineSprite.h>

// ī�޶� 1�� ������� ����.
// ���� :
class OldFilm : public GameEnginePostProcess
{
public:
	// constrcuter destructer
	OldFilm();
	~OldFilm();

	// delete Function
	OldFilm(const OldFilm& _Other) = delete;
	OldFilm(OldFilm&& _Other) noexcept = delete;
	OldFilm& operator=(const OldFilm& _Other) = delete;
	OldFilm& operator=(OldFilm&& _Other) noexcept = delete;

protected:
	void Start(GameEngineRenderTarget* _Target) override;
	void Effect(GameEngineRenderTarget* _Target, float _DeltaTime) override;

private:
	std::shared_ptr<GameEngineRenderUnit> OldUnit = nullptr;
	std::shared_ptr<GameEngineSprite> OldSprite = nullptr;

	float4 OldData;

	int Index = 0;

	void LevelChangeStart() override
	{
		ResultTarget->AddNewTexture(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT, GameEngineWindow::GetScreenSize(), float4::ZERONULL);
	}

	void LevelChangeEnd() override
	{
		ResultTarget->ReleaseTextures();
	}
};

