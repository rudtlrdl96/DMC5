#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class testarea : public GameEngineActor
{
public:
	// constrcuter destructer
	testarea();
	~testarea();

	// delete Function
	testarea(const testarea& _Other) = delete;
	testarea(testarea&& _Other) noexcept = delete;
	testarea& operator=(const testarea& _Other) = delete;
	testarea& operator=(testarea&& _Other) noexcept = delete;

protected:
	void Start() override;
private:
	std::shared_ptr<class GameEngineUIRenderer> testaaa = nullptr;
};

