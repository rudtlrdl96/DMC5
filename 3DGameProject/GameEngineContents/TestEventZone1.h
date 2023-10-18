#pragma once
#include "EventZone.h"

class TestEventZone1 : public EventZone
{
public:
	// construtor destructor
	TestEventZone1();
	~TestEventZone1();

	// delete Function
	TestEventZone1(const TestEventZone1& _Other) = delete;
	TestEventZone1(TestEventZone1&& _Other) noexcept = delete;
	TestEventZone1& operator=(const TestEventZone1& _Other) = delete;
	TestEventZone1& operator=(TestEventZone1&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

