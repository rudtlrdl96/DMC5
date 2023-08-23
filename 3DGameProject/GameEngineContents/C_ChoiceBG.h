#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// Ό³Έν :
class C_ChoiceBG : public GameEngineActor
{
public:
	// constrcuter destructer
	C_ChoiceBG();
	~C_ChoiceBG();

	// delete Function
	C_ChoiceBG(const C_ChoiceBG& _Other) = delete;
	C_ChoiceBG(C_ChoiceBG&& _Other) noexcept = delete;
	C_ChoiceBG& operator=(const C_ChoiceBG& _Other) = delete;
	C_ChoiceBG& operator=(C_ChoiceBG&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineUIRenderer> C_ChoiceBGRender = nullptr;

	float4 Scale = { 1600.0f,900.0f,0.0f };


};

