#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include "Char_ChoiceButton.h"
enum class ChoicePlayerType
{
	NONE,
	NERO,
	VERGIL
};
// Ό³Έν :
class Char_ChoiceUI : public GameEngineActor
{
public: 
	static Char_ChoiceUI* CharChoicePtr;
	// constrcuter destructer
	Char_ChoiceUI();
	~Char_ChoiceUI();

	// delete Function
	Char_ChoiceUI(const Char_ChoiceUI& _Other) = delete;
	Char_ChoiceUI(Char_ChoiceUI&& _Other) noexcept = delete;
	Char_ChoiceUI& operator=(const Char_ChoiceUI& _Other) = delete;
	Char_ChoiceUI& operator=(Char_ChoiceUI&& _Other) noexcept = delete;

	inline void SetCharTypeCallBack(std::function<void()> _CharTypeCallBack)
	{
		CharTypeCallBack = _CharTypeCallBack;
	}
	static ChoicePlayerType GetPlayerType()
	{
		return Type;
	}
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:
	void SetButtonIndex();
	std::shared_ptr<Char_ChoiceButton> Nero_ChoiceUIRender = nullptr;
	std::shared_ptr<Char_ChoiceButton> Vergil_ChoiceUIRender = nullptr;
	float4 WindowRatio = float4::ZERONULL;
	static ChoicePlayerType Type;
	float4 Scale = { 342.0f,627.0f,0.0f };
	float4 SeletScale = { 376.0f,690.0f,0.0f };
	float4 TextScale = { 20.0f,-150.0f,0.0f };
	float ScaleUpTime = 0.0f;
	float ScaleDownTime = 0.0f;
	float4 NeroPos = { -300.0f,0.0f,0.0f };
	float4 VergilPos = { 300.0f,0.0f,0.0f };
	std::function<void()> CharTypeCallBack = nullptr;

	int SelectIndex = 0;
};

