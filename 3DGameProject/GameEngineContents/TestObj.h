#pragma once
#include "StaticFieldMapObject.h"

class TestObj : public StaticFieldMapObject
{
public:
	// construtor destructor
	TestObj();
	~TestObj();

	// delete Function
	TestObj(const TestObj& _Other) = delete;
	TestObj(TestObj&& _Other) noexcept = delete;
	TestObj& operator=(const TestObj& _Other) = delete;
	TestObj& operator=(TestObj&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

