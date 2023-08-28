#pragma once
#include "BaseLevel.h"

// Ό³Έν :
class ShaderTestLevel : public BaseLevel
{
public:
	// constrcuter destructer
	ShaderTestLevel();
	~ShaderTestLevel();

	// delete Function
	ShaderTestLevel(const ShaderTestLevel& _Other) = delete;
	ShaderTestLevel(ShaderTestLevel&& _Other) noexcept = delete;
	ShaderTestLevel& operator=(const ShaderTestLevel& _Other) = delete;
	ShaderTestLevel& operator=(ShaderTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;

	void LevelChangeStart() override;

private:
	std::shared_ptr<class JudgementCut> Effect = nullptr;

};

