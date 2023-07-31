#pragma once
#include <GameEngineCore/GameEngineSpriteRenderer.h>


// Ό³Έν :
class MyContentSpriteRenderer : public GameEngineSpriteRenderer
{
public:
	void Start() override;

	float4 AColor;

public:
	// constrcuter destructer
	MyContentSpriteRenderer();
	~MyContentSpriteRenderer();

};

