#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// ���� :
class TitleBG : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleBG();
	~TitleBG();

	// delete Function
	TitleBG(const TitleBG& _Other) = delete;
	TitleBG(TitleBG&& _Other) noexcept = delete;
	TitleBG& operator=(const TitleBG& _Other) = delete;
	TitleBG& operator=(TitleBG&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineUIRenderer> TitleBGRender = nullptr;
	float4 Scale = { 1600.0f,900.0f,0.0f };
	std::shared_ptr<class GameEngineFontRenderer> TestFont;


};

