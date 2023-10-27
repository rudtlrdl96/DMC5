#pragma once
#include "EventZone.h"

class LevelChangeZone : public EventZone
{
public:
	// construtor destructor
	LevelChangeZone();
	~LevelChangeZone();

	// delete Function
	LevelChangeZone(const LevelChangeZone& _Other) = delete;
	LevelChangeZone(LevelChangeZone&& _Other) noexcept = delete;
	LevelChangeZone& operator=(const LevelChangeZone& _Other) = delete;
	LevelChangeZone& operator=(LevelChangeZone&& _Other) noexcept = delete;

	void ChangeLevel();

protected:
	void Start() override;
	std::string ChangeLevelName = std::string();

private:

};

