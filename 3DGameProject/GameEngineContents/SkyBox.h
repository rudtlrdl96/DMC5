#pragma once
#include "BaseStageActor.h"

class SkyBox : public BaseStageActor
{
public:
	// construtor destructor
	SkyBox();
	~SkyBox();

	// delete Function
	SkyBox(const SkyBox& _Other) = delete;
	SkyBox(SkyBox&& _Other) noexcept = delete;
	SkyBox& operator=(const SkyBox& _Other) = delete;
	SkyBox& operator=(SkyBox&& _Other) noexcept = delete;
	
	static std::shared_ptr<SkyBox> CreateSkyBox(GameEngineLevel* _Level, const std::string_view& _FBXName);

protected:

private:
	std::shared_ptr<class GameEngineFBXRenderer> Sky = nullptr;
};

