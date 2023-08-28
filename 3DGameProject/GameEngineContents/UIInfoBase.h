#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFont.h>
class UIInfoParameter
{
public:
	std::string _Text = "";
	std::string _SeletText = "";
	std::string _ExplaneText = "";
	int Index = 0;

};
// 설명 :
class GameEngineCollision;
class UIInfoBase : public GameEngineActor
{
public:
	// constrcuter destructer
	UIInfoBase();
	~UIInfoBase();

	// delete Function
	UIInfoBase(const UIInfoBase& _Other) = delete;
	UIInfoBase(UIInfoBase&& _Other) noexcept = delete;
	UIInfoBase& operator=(const UIInfoBase& _Other) = delete;
	UIInfoBase& operator=(UIInfoBase&& _Other) noexcept = delete;
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender()
	{
		return Render;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Base()
	{
		return Render_Base;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender_Bar()
	{
		return Render_Bar;
	}
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
	void SetUIText(const UIInfoParameter& _Paramter);
	void SetExplanePos(float4 _Pos)
	{
		ExPlanePos = _Pos;
	}


protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();

	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;//폰트 안떠서 임시
	std::shared_ptr<GameEngineUIRenderer> Render_Base = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Bar = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> Text = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SeletText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText_2 = nullptr;


	std::function<void()> Click;
	std::string Font = "DMC5Font";
	float4 ExPlanePos = float4::ZERONULL;
};

