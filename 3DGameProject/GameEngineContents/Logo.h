#pragma once
#include <GameEngineCore/GameEngineActor.h>
// 설명 :
class Logo : public GameEngineActor
{
public:
	// constrcuter destructer
	Logo();
	~Logo();

	// delete Function
	Logo(const Logo& _Other) = delete;
	Logo(Logo&& _Other) noexcept = delete;
	Logo& operator=(const Logo& _Other) = delete;
	Logo& operator=(Logo&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:

	std::shared_ptr<class GameEngineSpriteRenderer> CapComLogo = nullptr;
	std::shared_ptr<class GameEngineSpriteRenderer> PhysxLogo = nullptr;
	//변수
	bool Value = false;
	float Time = 0.0f;
	float Ratio = 0.5f;
	float4 Pos = {0.0f,0.0f,-1.0f};
};

