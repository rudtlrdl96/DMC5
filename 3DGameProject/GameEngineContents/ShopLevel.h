#pragma once
#include "BaseLevel.h"
// ���� :
class ShopLevel : public BaseLevel
{
public: 
	// constrcuter destructer
	ShopLevel();
	~ShopLevel();

	// delete Function
	ShopLevel(const ShopLevel& _Other) = delete;
	ShopLevel(ShopLevel&& _Other) noexcept = delete;
	ShopLevel& operator=(const ShopLevel& _Other) = delete;
	ShopLevel& operator=(ShopLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;
private:
	std::shared_ptr<class FadeEffect> FEffect;
	std::shared_ptr<class Nero_ShopUI> Nero_ShopUIPtr = nullptr;
	std::shared_ptr<class Vergil_ShopUI> Vergil_ShopUIPtr = nullptr;
};

