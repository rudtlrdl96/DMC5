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
	std::string _ExplaneText2 = "";
	int Index = 0;

};
// ���� :
class GameEngineCollision;
class InfoUIRender : public GameEngineActor
{
public:
	// constrcuter destructer
	InfoUIRender();
	~InfoUIRender();

	// delete Function
	InfoUIRender(const InfoUIRender& _Other) = delete;
	InfoUIRender(InfoUIRender&& _Other) noexcept = delete;
	InfoUIRender& operator=(const InfoUIRender& _Other) = delete;
	InfoUIRender& operator=(InfoUIRender&& _Other) noexcept = delete;
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
	std::shared_ptr<class GameEngineFontRenderer> GetSecoundExplaneText()
	{
		return ExplaneText_2;
	}
	void SetUIText(const UIInfoParameter& _Paramter);

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void FontCreate();

	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;//��Ʈ �ȶ��� �ӽ�
	std::shared_ptr<GameEngineUIRenderer> Render_Base = nullptr;
	std::shared_ptr<GameEngineUIRenderer> Render_Bar = nullptr;

	std::shared_ptr<class GameEngineFontRenderer> Text = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> SeletText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText = nullptr;
	std::shared_ptr<class GameEngineFontRenderer> ExplaneText_2 = nullptr;
	float4 WindowPos = float4::ZERONULL;

	std::function<void()> Click;
	std::string Font = "DMC5Font";
	float4 FontColor = { 0.67f,0.721f,0.720f };
};

