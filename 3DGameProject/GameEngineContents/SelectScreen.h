#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "ContentButton.h"
// ���� :
class SelectScreen : public GameEngineActor
{
public:
	// constrcuter destructer
	SelectScreen();
	~SelectScreen();

	// delete Function
	SelectScreen(const SelectScreen& _Other) = delete;
	SelectScreen(SelectScreen&& _Other) noexcept = delete;
	SelectScreen& operator=(const SelectScreen& _Other) = delete;
	SelectScreen& operator=(SelectScreen&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	//������ ��ư�� 3��������..
	std::shared_ptr<ContentButton> ContinueButton = nullptr;
	std::shared_ptr<ContentButton> OptionButton = nullptr;
	std::shared_ptr<ContentButton> ExitButton = nullptr;
};

