#pragma once
#include "StageBaseLevel.h"

class Location2_Level : public StageBaseLevel
{
public:
	// construtor destructor
	Location2_Level();
	~Location2_Level();

	// delete Function
	Location2_Level(const Location2_Level& _Other) = delete;
	Location2_Level(Location2_Level&& _Other) noexcept = delete;
	Location2_Level& operator=(const Location2_Level& _Other) = delete;
	Location2_Level& operator=(Location2_Level&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;

private:

};

