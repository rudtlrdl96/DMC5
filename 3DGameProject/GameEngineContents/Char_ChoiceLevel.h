#pragma once
#include "BaseLevel.h"
// Ό³Έν :
class Char_ChoiceLevel : public BaseLevel
{
public:
	// constrcuter destructer
	Char_ChoiceLevel();
	~Char_ChoiceLevel();

	// delete Function
	Char_ChoiceLevel(const Char_ChoiceLevel& _Other) = delete;
	Char_ChoiceLevel(Char_ChoiceLevel&& _Other) noexcept = delete;
	Char_ChoiceLevel& operator=(const Char_ChoiceLevel& _Other) = delete;
	Char_ChoiceLevel& operator=(Char_ChoiceLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render() {};

	void LevelChangeStart() override;
	void LevelChangeEnd() override;
private:
	std::shared_ptr<class FadeEffect> FEffect;

};

