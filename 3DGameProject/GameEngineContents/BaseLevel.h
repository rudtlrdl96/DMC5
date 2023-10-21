#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"

/*
	만약 여기에 네트워크 코드 말고
	다른 코드 추가하셔야 한다면
	네트워크 부분은 따로 분리시키겠습니다.
*/

class BaseLevel : public GameEngineLevel
{
public:
	//Net_LevelType 값으로 BaseLevel*를 얻어옵니다. (대신 자식쪽 Start에서 SetNetLevelType를 호출시켜 주어야 합니다)
	//static BaseLevel* GetNetLevel(Net_LevelType _Type);

	BaseLevel();
	~BaseLevel();

	BaseLevel(const BaseLevel& _Other) = delete;
	BaseLevel(BaseLevel&& _Other) noexcept = delete;
	BaseLevel& operator=(const BaseLevel& _Other) = delete;
	BaseLevel& operator=(const BaseLevel&& _Other) noexcept = delete;

	inline Net_LevelType GetNetLevelType() const
	{
		return NetLevelType;
	}


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void NetworkUpdate(float _DeltaTime) final;

	void LevelChangeStart() override;

	void SetNetLevelType(Net_LevelType _Type);

private:
	static std::map<Net_LevelType, BaseLevel*> AllNetLevel;

	Net_LevelType NetLevelType = Net_LevelType::UNKNOWN;
	float NetTimer = 0.f;
};

