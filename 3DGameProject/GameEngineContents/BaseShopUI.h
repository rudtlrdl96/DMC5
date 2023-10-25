#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class BaseShopUI :public GameEngineActor
{
public:
	// constrcuter destructer
	BaseShopUI();
	~BaseShopUI();

	// delete Function
	BaseShopUI(const BaseShopUI& _Other) = delete;
	BaseShopUI(BaseShopUI&& _Other) noexcept = delete;
	BaseShopUI& operator=(const BaseShopUI& _Other) = delete;
	BaseShopUI& operator=(BaseShopUI&& _Other) noexcept = delete;

protected:
	void Start();
	void Update(float _Delta);
	virtual void ShopOn() {}
	virtual void ShopOff() {}
	virtual void ApperCusterWindow(bool _Value) {}
private:

};

