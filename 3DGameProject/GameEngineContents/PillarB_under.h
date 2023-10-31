#pragma once
#include "StaticFieldMapObject.h"
class PillarB_under : public StaticFieldMapObject
{
public:
	// construtor destructor
	PillarB_under();
	~PillarB_under();

	// delete Function
	PillarB_under(const PillarB_under& _Other) = delete;
	PillarB_under(PillarB_under&& _Other) noexcept = delete;
	PillarB_under& operator=(const PillarB_under& _Other) = delete;
	PillarB_under& operator=(PillarB_under&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

