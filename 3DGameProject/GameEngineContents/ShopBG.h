#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineUIRenderer.h>

// Ό³Έν :
class ShopBG : public GameEngineActor
{
public:
	// constrcuter destructer
	ShopBG();
	~ShopBG();

	// delete Function
	ShopBG(const ShopBG& _Other) = delete;
	ShopBG(ShopBG&& _Other) noexcept = delete;
	ShopBG& operator=(const ShopBG& _Other) = delete;
	ShopBG& operator=(ShopBG&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	std::shared_ptr<GameEngineUIRenderer> ShopBGRender0 = nullptr;
	std::shared_ptr<GameEngineUIRenderer> ShopBGRender1 = nullptr;
	std::shared_ptr<GameEngineUIRenderer> ShopBGRender2 = nullptr;

	float4 Scale = { 1600.0f,900.0f,0.0f };

};

