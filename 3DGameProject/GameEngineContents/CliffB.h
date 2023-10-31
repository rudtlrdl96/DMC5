#pragma once
#include "StaticFieldMapObject.h"
class CliffB : public StaticFieldMapObject
{
public:
	// construtor destructor
	CliffB();
	~CliffB();

	// delete Function
	CliffB(const CliffB& _Other) = delete;
	CliffB(CliffB&& _Other) noexcept = delete;
	CliffB& operator=(const CliffB& _Other) = delete;
	CliffB& operator=(CliffB&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

