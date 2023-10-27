#pragma once
#include "LevelChangeZone.h"

class ToLocation11 : public LevelChangeZone
{
public:
	// construtor destructor
	ToLocation11();
	~ToLocation11();

	// delete Function
	ToLocation11(const ToLocation11& _Other) = delete;
	ToLocation11(ToLocation11&& _Other) noexcept = delete;
	ToLocation11& operator=(const ToLocation11& _Other) = delete;
	ToLocation11& operator=(ToLocation11&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

