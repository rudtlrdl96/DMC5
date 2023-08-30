#pragma once
#include "BaseLevel.h"
// Ό³Έν :
class Nero_ShopLevel : public BaseLevel
{
public:
	// constrcuter destructer
	Nero_ShopLevel();
	~Nero_ShopLevel();

	// delete Function
	Nero_ShopLevel(const Nero_ShopLevel& _Other) = delete;
	Nero_ShopLevel(Nero_ShopLevel&& _Other) noexcept = delete;
	Nero_ShopLevel& operator=(const Nero_ShopLevel& _Other) = delete;
	Nero_ShopLevel& operator=(Nero_ShopLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;
private:
	std::shared_ptr<class FadeEffect> FEffect;

};

