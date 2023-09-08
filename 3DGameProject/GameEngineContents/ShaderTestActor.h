#pragma once
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "EffectFBXRenderer.h"

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
	std::shared_ptr<EffectFBXRenderer> TestRenderer = nullptr;


};

