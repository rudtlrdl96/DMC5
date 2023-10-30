#pragma once
#include "StageBaseLevel.h"

class Location19_Level : public StageBaseLevel
{
public:
	// construtor destructor
	Location19_Level();
	~Location19_Level();

	// delete Function
	Location19_Level(const Location19_Level& _Other) = delete;
	Location19_Level(Location19_Level&& _Other) noexcept = delete;
	Location19_Level& operator=(const Location19_Level& _Other) = delete;
	Location19_Level& operator=(Location19_Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;

private:
	void CreateEventZone();

};

