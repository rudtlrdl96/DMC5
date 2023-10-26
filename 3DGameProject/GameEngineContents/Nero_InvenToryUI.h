#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "ContentsEnum.h"
// Ό³Έν :
class Nero_InvenToryUI :public GameEngineActor
{
public:
	//static std::vector<std::shared_ptr<Nero_InvenToryUI>> Invens;
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
	static void IsAddInvenItem(bool _Value)
	{
		IsAddInvenArm = _Value;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void AddInvenArm();
	std::shared_ptr<class GameEngineUIRenderer> InvenBaseRender = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ItemRender_0 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ItemRender_1 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ItemRender_2 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> ItemRender_3 = nullptr;

	float4 Scale = { 440.0f,110.f,0.0f };
	DevilBreaker CurDevilBreaker = DevilBreaker::None;
	std::vector<DevilBreaker> InvenDevil;
	std::vector<std::shared_ptr<class GameEngineUIRenderer>> DevilBreakerRender;
	static bool IsAddInvenArm;
};

