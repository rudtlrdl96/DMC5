#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include "ContentsEnum.h"

/*
	���� ���⿡ ��Ʈ��ũ �ڵ� ����
	�ٸ� �ڵ� �߰��ϼž� �Ѵٸ�
	��Ʈ��ũ �κ��� ���� �и���Ű�ڽ��ϴ�.
*/

class BaseLevel : public GameEngineLevel
{
public:
	//Net_LevelType ������ BaseLevel*�� ���ɴϴ�. (��� �ڽ��� Start���� SetNetLevelType�� ȣ����� �־�� �մϴ�)
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

