#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
class StringParameter
{
public:
	std::string _Text = "";
	std::string _SeletText = "";
	std::string _ExplaneText = "";
	int Index = 0;

};
// 설명 :
class GameEngineCollision;
class TitleButton : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleButton();
	~TitleButton();

	// delete Function
	TitleButton(const TitleButton& _Other) = delete;
	TitleButton(TitleButton&& _Other) noexcept = delete;
	TitleButton& operator=(const TitleButton& _Other) = delete;
	TitleButton& operator=(TitleButton&& _Other) noexcept = delete;
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender()
	{
		return Render;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Select()
	{
		return Render_Select;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Select2()
	{
		return Render_Select2;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Enter()
	{
		return Render_Enter;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Enter2()
	{
		return Render_Enter2;
	}
	void SetUIText(const StringParameter& _Paramter);
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
	void FontCreate();
	void EnterKeyRender(float _Delta);
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;//폰트 안떠서 임시
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Select2 = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Enter = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Enter2 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> Text = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SeletText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText = nullptr;
	std::function<void()> Click;

	std::string Font = "DMC5Font";

	bool IsSelect = false;
	bool IsValue = false;
	bool SwichValue = false;
	bool EnterValue = false;
	float fillTime = 0.0f;
	float fallTime = 0.0f;
	float EnterTime = 0.0f;
	float Ratio = 0.1f;
};

