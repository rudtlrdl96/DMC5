#pragma once
#include "EventZone.h"

class Location2EndZone : public EventZone
{
public:
	// construtor destructor
	Location2EndZone();
	~Location2EndZone();

	// delete Function
	Location2EndZone(const Location2EndZone& _Other) = delete;
	Location2EndZone(Location2EndZone&& _Other) noexcept = delete;
	Location2EndZone& operator=(const Location2EndZone& _Other) = delete;
	Location2EndZone& operator=(Location2EndZone&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

