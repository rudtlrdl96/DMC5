#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "StageDatas.h"

class BaseStageActor : public GameEngineActor
{
public:
	// construtor destructor
	BaseStageActor();
	~BaseStageActor();

	// delete Function
	BaseStageActor(const BaseStageActor& _Other) = delete;
	BaseStageActor(BaseStageActor&& _Other) noexcept = delete;
	BaseStageActor& operator=(const BaseStageActor& _Other) = delete;
	BaseStageActor& operator=(BaseStageActor&& _Other) noexcept = delete;

protected:
	static bool alphaCheck(const std::string_view& _FBXName);
	static bool waterCheck(const std::string_view& _FBXName);
private:

};

