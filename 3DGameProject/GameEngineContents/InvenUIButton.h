#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFont.h>
// Ό³Έν :
class GameEngineCollision;
class InvenUIButton : public GameEngineActor
{
public:
	// constrcuter destructer
	InvenUIButton();
	~InvenUIButton();

	// delete Function
	InvenUIButton(const InvenUIButton& _Other) = delete;
	InvenUIButton(InvenUIButton&& _Other) noexcept = delete;
	InvenUIButton& operator=(const InvenUIButton& _Other) = delete;
	InvenUIButton& operator=(InvenUIButton&& _Other) noexcept = delete;
	void SetEvent(std::function<void()> _Click)
	{
		Click = _Click;
	}
	std::shared_ptr<GameEngineUIRenderer> GetRender()
	{
		return Render;
	}
	//
	void SetLerpValue(float4 _StartPos, float4 _EndPos)
	{
		StartPos = _StartPos;
		EndPos = _EndPos;
	}

protected:
	void Start() override;
	void Update(float _Delta) override;

private:
	void LerpTexture(float4 _StartPos , float4 _EndPos , float _Ratio, float _Delta);
	std::shared_ptr<GameEngineUIRenderer> Render = nullptr;
	std::function<void()> Click;
	float4 StartPos = float4::ZERONULL;
	float4 EndPos = float4::ZERONULL;

	bool SwichValue = false;
	float Ratio = 1.5f;
	float Time = 0.0f;
	float Speed = 0.0f;
};

