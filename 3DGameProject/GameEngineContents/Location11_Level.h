#pragma once
#include "StageBaseLevel.h"

class Location11_Level : public StageBaseLevel
{
public:
	// construtor destructor
	Location11_Level();
	~Location11_Level();

	// delete Function
	Location11_Level(const Location11_Level& _Other) = delete;
	Location11_Level(Location11_Level&& _Other) noexcept = delete;
	Location11_Level& operator=(const Location11_Level& _Other) = delete;
	Location11_Level& operator=(Location11_Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;

private:
	// 결과화면 레벨로 이동합니다. 레벨 전환전 스크린샷을 촬영합니다.
	void LevelChangeToResultLevel();
	void CreateEventZone();
	void CutSceneStart();
	void BossDeathEvent();

	std::shared_ptr<class CavaliereAngelo> BossMonster = nullptr;
};

