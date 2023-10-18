#pragma once
#include "EventZone.h"

class TestEventZone0 : public EventZone
{
public:
	// construtor destructor
	TestEventZone0();
	~TestEventZone0();

	// delete Function
	TestEventZone0(const TestEventZone0& _Other) = delete;
	TestEventZone0(TestEventZone0&& _Other) noexcept = delete;
	TestEventZone0& operator=(const TestEventZone0& _Other) = delete;
	TestEventZone0& operator=(TestEventZone0&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

