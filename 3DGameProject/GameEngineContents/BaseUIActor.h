#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class BaseUIActor : public GameEngineActor
{
public:
	// constrcuter destructer
	BaseUIActor();
	~BaseUIActor();

	// delete Function
	BaseUIActor(const BaseUIActor& _Other) = delete;
	BaseUIActor(BaseUIActor&& _Other) noexcept = delete;
	BaseUIActor& operator=(const BaseUIActor& _Other) = delete;
	BaseUIActor& operator=(BaseUIActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;
private:
	void LoadMesh();
};

