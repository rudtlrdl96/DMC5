#pragma once
#include "UIMeshRenderer.h"
#include "EffectRenderer.h"

// Ό³Έν :
class ShaderTestActor : public GameEngineActor
{
public:
	// constrcuter destructer
	ShaderTestActor();
	~ShaderTestActor();

	// delete Function
	ShaderTestActor(const ShaderTestActor& _Other) = delete;
	ShaderTestActor(ShaderTestActor&& _Other) noexcept = delete;
	ShaderTestActor& operator=(const ShaderTestActor& _Other) = delete;
	ShaderTestActor& operator=(ShaderTestActor&& _Other) noexcept = delete;

	void Start() override;

	void InitTest(int _Index);


protected:

private:
	std::shared_ptr<class GameEngineFBXRenderer> TestRenderer = nullptr;
	//std::shared_ptr<EffectRenderer> TestRenderer = nullptr;

	void DrawEditor() override;
};

