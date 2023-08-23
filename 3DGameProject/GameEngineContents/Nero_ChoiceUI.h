#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// Ό³Έν :
class Nero_ChoiceUI : public GameEngineActor
{
public:
	// constrcuter destructer
	Nero_ChoiceUI();
	~Nero_ChoiceUI();

	// delete Function
	Nero_ChoiceUI(const Nero_ChoiceUI& _Other) = delete;
	Nero_ChoiceUI(Nero_ChoiceUI&& _Other) noexcept = delete;
	Nero_ChoiceUI& operator=(const Nero_ChoiceUI& _Other) = delete;
	Nero_ChoiceUI& operator=(Nero_ChoiceUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineUIRenderer> Nero_ChoiceUIRender = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Nero_ChoiceUIRender2 = nullptr;

	float4 Scale = { 342.0f,627.0f,0.0f };
	float4 SeletScale = { 376.0f,690.0f,0.0f };
	float ScaleUpTime = 0.0f;
	float ScaleDownTime = 0.0f;
	float4 Pos = { 0.0f,0.0f,0.0f };
	int TestIndex = 0;
};

