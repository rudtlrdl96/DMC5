#pragma once

#include "../GameEngineCore/ThirdParty/NvCloth/inc/Factory.h"

// Ό³Έν :
class GameEngineCloth
{
public:
	// constrcuter destructer
	GameEngineCloth();
	~GameEngineCloth();

	// delete Function
	GameEngineCloth(const GameEngineCloth& _Other) = delete;
	GameEngineCloth(GameEngineCloth&& _Other) noexcept = delete;
	GameEngineCloth& operator=(const GameEngineCloth& _Other) = delete;
	GameEngineCloth& operator=(GameEngineCloth&& _Other) noexcept = delete;

	static void CreateFactory();
	static void CreateCloth(std::shared_ptr<class GameEngineFBXRenderer> _Renderer);

protected:

private:
	void Release();

	static nv::cloth::Factory* m_pFactory;
	static nv::cloth::Fabric* m_pFabric;
};

