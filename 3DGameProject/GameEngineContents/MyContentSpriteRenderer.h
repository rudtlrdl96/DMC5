#pragma once
#include <GameEngineCore/GameEngineSpriteRenderer.h>


// ���� :
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

