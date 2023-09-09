#pragma once

#include "GameEngineRenderTarget.h"

// Ό³Έν :
class GameEngineCubeMap : public GameEngineRenderTarget, std::enable_shared_from_this<GameEngineCubeMap>
{
	friend class GameEngineCoreWindow;
	friend class GameEngineCore;
	friend class GameEngineLevel;

public:
	// constrcuter destructer
	GameEngineCubeMap();
	~GameEngineCubeMap();

	// delete Function
	GameEngineCubeMap(const GameEngineCubeMap& _Other) = delete;
	GameEngineCubeMap(GameEngineCubeMap&& _Other) noexcept = delete;
	GameEngineCubeMap& operator=(const GameEngineCubeMap& _Other) = delete;
	GameEngineCubeMap& operator=(GameEngineCubeMap&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineCubeMap> Create()
	{
		std::shared_ptr<GameEngineCubeMap> NewRenderTarget = std::dynamic_pointer_cast<GameEngineCubeMap>(GameEngineResource::CreateUnNamed());
		return NewRenderTarget;
	}

	void CreateCube()
	{
		GameEngineCubeMap::ResCreate();
	}

	void CreateCubeDepthTexture(int _Index = 0);

protected:

private:
	void ResCreate();

};

