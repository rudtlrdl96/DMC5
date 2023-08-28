#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFont.h>
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
class UIButton : public GameEngineActor
{
public:
	// constrcuter destructer
	UIButton();
	~UIButton();

	// delete Function
	UIButton(const UIButton& _Other) = delete;
	UIButton(UIButton&& _Other) noexcept = delete;
	UIButton& operator=(const UIButton& _Other) = delete;
	UIButton& operator=(UIButton&& _Other) noexcept = delete;
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


	//
	std::shared_ptr<class GameEngineFontRenderer> GetText()
	{
		return Text;
	}
	std::shared_ptr<class GameEngineFontRenderer> GetSeletText()
	{
		return SeletText;
	}
	std::shared_ptr<class GameEngineFontRenderer> GetExplaneText()
	{
		return ExplaneText;
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
	void SetExplanePos(float4 _Pos)
	{
		ExPlanePos = _Pos;
	}
	void SetExFontFlag(FW1_TEXT_FLAG _Flag)
	{
		Flag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();
	void EnterKeyRender(float _Delta);
	void RenderOnOff();
	void SetFontPos();

	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;//폰트 안떠서 임시
	std::shared_ptr<GameEngineUIRenderer> Render_Select = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Select2 = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Enter = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Enter2 = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> Text = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SeletText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText_1 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText_2 = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText_3 = nullptr;

	std::function<void()> Click;
	std::string Font = "DMC5Font";
	float4 ExPlanePos = float4::ZERONULL;
	FW1_TEXT_FLAG Flag = FW1_CENTER;
	bool IsSelect = false;
	bool IsValue = false;
	bool SwichValue = false;
	float fillTime = 0.0f;
	float fallTime = 0.0f;
	float Ratio = 0.1f;
};

