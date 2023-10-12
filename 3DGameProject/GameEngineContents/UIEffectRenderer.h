#pragma once
#include "EffectRenderer.h"
// Ό³Έν :
class UIEffectRenderer : public EffectRenderer
{
public:
	// constrcuter destructer
	UIEffectRenderer();
	~UIEffectRenderer();

	// delete Function
	UIEffectRenderer(const UIEffectRenderer& _Other) = delete;
	UIEffectRenderer(UIEffectRenderer&& _Other) noexcept = delete;
	UIEffectRenderer& operator=(const UIEffectRenderer& _Other) = delete;
	UIEffectRenderer& operator=(UIEffectRenderer&& _Other) noexcept = delete;

protected:
	void Start() override;
private:

};

