#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Shop_EnterWindow : public GameEngineActor
{
public:
	// constrcuter destructer
	Shop_EnterWindow();
	~Shop_EnterWindow();

	// delete Function
	Shop_EnterWindow(const Shop_EnterWindow& _Other) = delete;
	Shop_EnterWindow(Shop_EnterWindow&& _Other) noexcept = delete;
	Shop_EnterWindow& operator=(const Shop_EnterWindow& _Other) = delete;
	Shop_EnterWindow& operator=(Shop_EnterWindow&& _Other) noexcept = delete;

protected:
	void Start();
	void Update(float _Delta);
private:
	void ApperWindow(float _Delta);
	void DisApperWindow(float _Delta);
	std::shared_ptr<class GameEngineUIRenderer> ShopWindowRender = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> EnterText = nullptr;
	float Time = 0.0f;
	bool IsApper = false;
	bool IsDisApper = false;
};

