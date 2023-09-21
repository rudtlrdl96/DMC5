#pragma once
#include "EventZone.h"

class TestEventZone : public EventZone
{
public:
	// construtor destructor
	TestEventZone();
	~TestEventZone();

	// delete Function
	TestEventZone(const TestEventZone& _Other) = delete;
	TestEventZone(TestEventZone&& _Other) noexcept = delete;
	TestEventZone& operator=(const TestEventZone& _Other) = delete;
	TestEventZone& operator=(TestEventZone&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

