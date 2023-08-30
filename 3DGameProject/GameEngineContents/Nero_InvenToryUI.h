#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Nero_InvenToryUI :public GameEngineActor
{
public:
	static std::vector<std::shared_ptr<Nero_InvenToryUI>> Invens;
	// constrcuter destructer
	Nero_InvenToryUI();
	~Nero_InvenToryUI();

	// delete Function
	Nero_InvenToryUI(const Nero_InvenToryUI& _Other) = delete;
	Nero_InvenToryUI(Nero_InvenToryUI&& _Other) noexcept = delete;
	Nero_InvenToryUI& operator=(const Nero_InvenToryUI& _Other) = delete;
	Nero_InvenToryUI& operator=(Nero_InvenToryUI&& _Other) noexcept = delete;
	std::shared_ptr<class GameEngineUIRenderer> GetRender()
	{
		return InvenBaseRender;
	}
	static void CreateNeroInven(GameEngineLevel* _Level);
	
protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	std::shared_ptr<class GameEngineUIRenderer> InvenBaseRender = nullptr;
};

