#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <string>
// Ό³Έν :
class DebugKeyUI : public GameEngineActor
{
public:
	// constrcuter destructer
	DebugKeyUI();
	~DebugKeyUI();

	// delete Function
	DebugKeyUI(const DebugKeyUI& _Other) = delete;
	DebugKeyUI(DebugKeyUI&& _Other) noexcept = delete;
	DebugKeyUI& operator=(const DebugKeyUI& _Other) = delete;
	DebugKeyUI& operator=(DebugKeyUI&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void KeyDebugOff();
	void KeyDebugOn();
	std::shared_ptr<class GameEngineUIRenderer> KeyRender0 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> KeyRender1 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> KeyRender2 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> KeyRender3 = nullptr;
	std::shared_ptr<class GameEngineUIRenderer> KeyRender4 = nullptr;
	void UpdateKeyRender();
	int Index = 0;
	int Count = 0;
	std::list<std::string> InputKeys;
	std::vector<std::shared_ptr<class GameEngineUIRenderer>> KeyRenders;
	float4 Pos = { -540.0f,-315.0f,0.0f };
	float4 SubPos = { 50.0f,0.0f,0.0f };
};

