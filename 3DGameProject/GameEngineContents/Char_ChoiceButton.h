#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
class Char_ChoiceParameter
{
public:
	std::string _Text = "";
	std::string _SeletText = "";
	std::string _ExplaneText = "";
	int Index = 0;

};
// 설명 :
class GameEngineCollision;
class Char_ChoiceButton : public GameEngineActor
{
public:
	// constrcuter destructer
	Char_ChoiceButton();
	~Char_ChoiceButton();

	// delete Function
	Char_ChoiceButton(const Char_ChoiceButton& _Other) = delete;
	Char_ChoiceButton(Char_ChoiceButton&& _Other) noexcept = delete;
	Char_ChoiceButton& operator=(const Char_ChoiceButton& _Other) = delete;
	Char_ChoiceButton& operator=(Char_ChoiceButton&& _Other) noexcept = delete;
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}
	std::shared_ptr<GameEngineUIRenderer> GetNoneSelectRender()
	{
		return NoneSelectRender;
	}
	std::shared_ptr<GameEngineUIRenderer> GetSelectRender()
	{
		return SelectRender;
	}
	std::shared_ptr<GameEngineUIRenderer> GetCharRender()
	{
		return CharRender;
	}
	std::shared_ptr<GameEngineUIRenderer> GetTextNoneRender()
	{
		return TextNoneRender;
	}
	std::shared_ptr<GameEngineUIRenderer> GetTextSelectRender()
	{
		return TextSelectRender;
	}
	void SetUIText(const Char_ChoiceParameter& _Paramter);
	void SetSelectValue(bool _Value)
	{
		IsSelect = _Value;
	}
	bool GetIsValue()
	{
		return IsValue;
	}
	void SetIsValue(bool _Value)
	{
		IsValue = _Value;
	}
protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	float4 SizeUpCalCulation(float4 _size , float4 _size2 , float Ratio);
	void FontCreate();
	void SetSaturation();

	std::shared_ptr<GameEngineUIRenderer> NoneSelectRender = nullptr;//폰트 안떠서 임시
	std::shared_ptr<GameEngineUIRenderer> SelectRender = nullptr;
	std::shared_ptr<GameEngineUIRenderer> CharRender = nullptr;
	std::shared_ptr<GameEngineUIRenderer> TextNoneRender= nullptr;
	std::shared_ptr<GameEngineUIRenderer> TextSelectRender = nullptr;
	std::shared_ptr<GameEngineUIRenderer> TextBarRender = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> NameText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SelectText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText = nullptr;
	std::function<void()> Click;

	std::string Font = "DMC5Font";
	float4 WindowRatio = float4::ZERONULL;
	bool IsSelect = false;
	bool IsValue = false;
	bool SwichValue = false;
	float ScaleUpTime = 0.0f;
	float ScaleDownTime = 0.0f;
	float4 Scale = { 342.0f,627.0f,0.0f };
	float4 SeletScale = { 376.0f,690.0f,0.0f };
	float4 Char_Scale = { 700.0f,700.0f,0.0f };
	float4 Char_SeletScale = { 770.0f,770.0f,0.0f };
	float4 Char_TextScale = { 376.0f,93.0f,0.0f };
	float4 Char_TextSeletScale = { 413.0f,102.0f,0.0f };
	float4 Char_BarScale = { 204.0f,4.0f,0.0f };
	float4 Char_BarScaleUp = { 224.0f,4.4f,0.0f };
	float4 SelcetBarScale = { 413.0f,102.0f,0.0f };
	float EnterTime = 0.0f;
	float Ratio = 0.1f;
};

