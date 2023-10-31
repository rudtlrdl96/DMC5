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
	inline void SetChangeLevel(const std::string_view& _LevelName)
	{
		ChangeLevelName = _LevelName;
	}

protected:
	void Start() override;
	std::string ChangeLevelName = std::string();
	std::vector<std::shared_ptr<class EffectRenderer>> DistortionEffect = {};

private:

};

